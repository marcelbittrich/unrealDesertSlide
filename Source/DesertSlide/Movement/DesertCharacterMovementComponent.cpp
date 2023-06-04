// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertCharacterMovementComponent.h"

#include "DesertSlideCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Math/UnrealMathUtility.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#pragma region Saved Move

bool UDesertCharacterMovementComponent::FSavedMove_Desert::CanCombineWith(const FSavedMovePtr& NewMove,
                                                                          ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_Desert* NewDesertMove = static_cast<FSavedMove_Desert*>(NewMove.Get());

	// If you start to sprint, you dont want to combine the movements.
	if (Saved_bWantsToSprint != NewDesertMove->Saved_bWantsToSprint)
	{
		return false;
	}
	
	// In case we want to combine, the super function will handle that.
	return Super::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UDesertCharacterMovementComponent::FSavedMove_Desert::Clear()
{
	Super::Clear();

	Saved_bWantsToSprint = 0;
}

uint8 UDesertCharacterMovementComponent::FSavedMove_Desert::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (Saved_bWantsToSprint)
	{
		Result |= FLAG_Custom_0;
	}

	return Result;
}

void UDesertCharacterMovementComponent::FSavedMove_Desert::SetMoveFor(ACharacter* C, float InDeltaTime,
	FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UDesertCharacterMovementComponent* CharacterMovement = Cast<UDesertCharacterMovementComponent>(C->GetCharacterMovement());

	Saved_bWantsToSprint = CharacterMovement->Safe_bWantsToSprint;
	Saved_bPrevWantsToCrouch = CharacterMovement->Safe_bPrevWantsToCrouch;
}

void UDesertCharacterMovementComponent::FSavedMove_Desert::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);
	
	UDesertCharacterMovementComponent* CharacterMovement = Cast<UDesertCharacterMovementComponent>(C->GetCharacterMovement());

	CharacterMovement->Safe_bWantsToSprint = Saved_bWantsToSprint;
	CharacterMovement->Safe_bPrevWantsToCrouch = Saved_bPrevWantsToCrouch;
}

#pragma endregion

#pragma region Client Network Prediction Data

UDesertCharacterMovementComponent::FNetworkPredictionData_Client_Desert::FNetworkPredictionData_Client_Desert(
	const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{
	// Just calling the Super constructor.
}

FSavedMovePtr UDesertCharacterMovementComponent::FNetworkPredictionData_Client_Desert::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Desert());
}

FNetworkPredictionData_Client* UDesertCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr)

	if (ClientPredictionData == nullptr)
	{
		// To create the prediction data if not available, it has to const_cast away the const of the component,
		// since this is a const function.
		UDesertCharacterMovementComponent* MutableThis = const_cast<UDesertCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Desert(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}
	
	return ClientPredictionData;
}

#pragma endregion

#pragma region CMC

UDesertCharacterMovementComponent::UDesertCharacterMovementComponent()
{

}

void UDesertCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SlideEnterSpeed = WalkBaseMaxSpeed + SlideEnterWalkSpeedOffset;
	DesertSlideCharacterOwner = Cast<ADesertSlideCharacter>(GetOwner());
}

void UDesertCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	
	Safe_bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UDesertCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	
	
	if (MovementMode == MOVE_Walking || MovementMode == MOVE_Custom)
	{
		// MaxWalkSpeed is used in CMOVE_Slide as exit value.
		float SlopeFactor = GetGroundSlopeFactor();
		MaxWalkSpeed = WalkBaseMaxSpeed + SlopeFactor * WalkSlopeSpeedOffset;
	}
	
	if (MovementMode == MOVE_Walking)
	{
		float SlopeFactor = GetGroundSlopeFactor();
		MaxAcceleration = WalkBaseAcceleration + SlopeFactor * WalkSlopeAcceleration;
	}

	Safe_bPrevWantsToCrouch = bWantsToCrouch;
}

bool UDesertCharacterMovementComponent::IsMovingOnGround() const
{
	// Adds Slide as a on ground movement.
	return Super::IsMovingOnGround() || IsCustomMovementMode(CMOVE_Slide);
}

bool UDesertCharacterMovementComponent::CanCrouchInCurrentState() const
{
	// Prevents crouching in air.
	return Super::CanCrouchInCurrentState() && IsMovingOnGround();
}

float UDesertCharacterMovementComponent::GetMaxSpeed() const
{
	if (MovementMode != MOVE_Custom) return Super::GetMaxSpeed();

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		return SlideMaxSpeed;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
		return -1.f;
	}
}
float UDesertCharacterMovementComponent::GetMaxBrakingDeceleration() const
{
	if (MovementMode != MOVE_Custom) return Super::GetMaxBrakingDeceleration();

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		return SlideBrakingDeceleration;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
		return -1.f;
	}
}

void UDesertCharacterMovementComponent::CalcSlideVelocity(float DeltaTime, float Friction, bool bFluid,
	float BrakingDeceleration)
{
	// Do not update velocity when using root motion or when SimulatedProxy and not simulating root motion - SimulatedProxy are repped their Velocity
	if (!HasValidData() || HasAnimRootMotion() || DeltaTime < MIN_TICK_TIME || (CharacterOwner && CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy && !bWasSimulatingRootMotion))
	{
		return;
	}

	Friction = FMath::Max(0.f, Friction);
	const float MaxAccel = GetMaxAcceleration();
	float MaxSpeed = GetMaxSpeed();
	
	// Check if path following requested movement
	bool bZeroRequestedAcceleration = true;
	FVector RequestedAcceleration = FVector::ZeroVector;
	float RequestedSpeed = 0.0f;
	if (ApplyRequestedMove(DeltaTime, MaxAccel, MaxSpeed, Friction, BrakingDeceleration, RequestedAcceleration, RequestedSpeed))
	{
		bZeroRequestedAcceleration = false;
	}

	if (bForceMaxAccel)
	{
		// Force acceleration at full speed.
		// In consideration order for direction: Acceleration, then Velocity, then Pawn's rotation.
		if (Acceleration.SizeSquared() > UE_SMALL_NUMBER)
		{
			Acceleration = Acceleration.GetSafeNormal() * MaxAccel;
		}
		else 
		{
			Acceleration = MaxAccel * (Velocity.SizeSquared() < UE_SMALL_NUMBER ? UpdatedComponent->GetForwardVector() : Velocity.GetSafeNormal());
		}

		AnalogInputModifier = 1.f;
	}

	// Path following above didn't care about the analog modifier, but we do for everything else below, so get the fully modified value.
	// Use max of requested speed and max speed if we modified the speed in ApplyRequestedMove above.
	const float MaxInputSpeed = FMath::Max(MaxSpeed * AnalogInputModifier, GetMinAnalogSpeed());
	MaxSpeed = FMath::Max(RequestedSpeed, MaxInputSpeed);

	// Apply braking or deceleration
	const bool bZeroAcceleration = Acceleration.IsZero();
	const bool bVelocityOverMax = IsExceedingMaxSpeed(MaxSpeed);
	
	// Only apply braking if there is no acceleration, or we are over our max speed and need to slow down to it.
	if ((bZeroAcceleration && bZeroRequestedAcceleration) || bVelocityOverMax)
	{
		const FVector OldVelocity = Velocity;

		const float ActualBrakingFriction = (bUseSeparateBrakingFriction ? BrakingFriction : Friction);
		ApplyVelocityBraking(DeltaTime, ActualBrakingFriction, BrakingDeceleration);
	
		// Don't allow braking to lower us below max speed if we started above it.
		if (bVelocityOverMax && Velocity.SizeSquared() < FMath::Square(MaxSpeed) && FVector::DotProduct(Acceleration, OldVelocity) > 0.0f)
		{
			Velocity = OldVelocity.GetSafeNormal() * MaxSpeed;
		}
	}
	else if (!bZeroAcceleration)
	{
		// Friction affects our ability to change direction. This is only done for input acceleration, not path following.
		const FVector AccelDir = Acceleration.GetSafeNormal();
		const float VelSize = Velocity.Size();
		
		// const float ActualBrakingFriction = (bUseSeparateBrakingFriction ? BrakingFriction : Friction);
		Velocity = Velocity - (Velocity - AccelDir * VelSize) * FMath::Min(DeltaTime * Friction, 1.f);
	}

	// Apply fluid friction
	if (bFluid)
	{
		const float ActualBrakingFriction = (bUseSeparateBrakingFriction ? BrakingFriction : Friction);
		Velocity = Velocity * (1.f - FMath::Min(ActualBrakingFriction * DeltaTime, 1.f));
	}

	// Apply input acceleration
	if (!bZeroAcceleration)
	{
		const float NewMaxInputSpeed = IsExceedingMaxSpeed(MaxInputSpeed) ? Velocity.Size() : MaxInputSpeed;
		Velocity += Acceleration * DeltaTime;
		Velocity = Velocity.GetClampedToMaxSize(NewMaxInputSpeed);
	}

	// Apply additional requested acceleration
	if (!bZeroRequestedAcceleration)
	{
		const float NewMaxRequestedSpeed = IsExceedingMaxSpeed(RequestedSpeed) ? Velocity.Size() : RequestedSpeed;
		Velocity += RequestedAcceleration * DeltaTime;
		Velocity = Velocity.GetClampedToMaxSize(NewMaxRequestedSpeed);
	}

	if (bUseRVOAvoidance)
	{
		CalcAvoidanceVelocity(DeltaTime);
	}
}

void UDesertCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DisplayDebugMessages();
}

void UDesertCharacterMovementComponent::DisplayDebugMessages()
{
	if (!bDebug) return;
	
	FString VelocityString = FString::SanitizeFloat(Velocity.Size());
	GEngine->AddOnScreenDebugMessage(1,1,FColor::Green, "Speed: " + VelocityString);

	FString MovementModeString = GetMovementName();
	GEngine->AddOnScreenDebugMessage(2,1,FColor::Green, "Mode: " + MovementModeString);
	GEngine->AddOnScreenDebugMessage(3,1,FColor::Green, "Slope: " + FString::SanitizeFloat(GetGroundSlopeFactor()));
	GEngine->AddOnScreenDebugMessage(4,1,FColor::Green, "Acc: " + FString::SanitizeFloat(Acceleration.Size()));
	uint8 CanCrouch = NavAgentProps.bCanCrouch;
	GEngine->AddOnScreenDebugMessage(6,1,FColor::Green, "CanCrouch: " + FString::FromInt(CanCrouch));

	GEngine->AddOnScreenDebugMessage(10,1,FColor::Green, "MaxAcc: " + FString::SanitizeFloat(GetMaxAcceleration()));
	GEngine->AddOnScreenDebugMessage(11,1,FColor::Green, "MaxVel: " + FString::SanitizeFloat(GetMaxSpeed()));
	GEngine->AddOnScreenDebugMessage(12,1,FColor::Green, "GroundFriction: " + FString::SanitizeFloat(GroundFriction));
}

void UDesertCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	bool bReachedSlidingSpeed = (MovementMode == MOVE_Walking) && Velocity.SizeSquared() > FMath::Square(SlideEnterSpeed);
	bool bWantsToSlide = (MovementMode == MOVE_Walking) && bWantsToCrouch;
	if (bReachedSlidingSpeed || bWantsToSlide)
	{
		if (CanSlide())
		{
			SetMovementMode(MOVE_Custom, CMOVE_Slide);
		}
	}
	
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UDesertCharacterMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);
}

void UDesertCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		PhysSlide(deltaTime, Iterations);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Invalid Movement Mode"));
		break;
	}
}

void UDesertCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode,
	uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	
	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_Slide) ExitSlide();
	if (IsCustomMovementMode(CMOVE_Slide)) EnterSlide();
}

float UDesertCharacterMovementComponent::GetGroundSlopeFactor()
{
	FHitResult Ground;
	if (GetSlideSurface(Ground))
	{
		FVector HorizontalVelocity = Velocity;
		HorizontalVelocity.Z = 0;

		if (bDebug)
		{
			DrawDebugLine(GetWorld(), Ground.ImpactPoint, Ground.ImpactPoint + Ground.ImpactNormal * 100, FColor::Green, false, -1, 0, 5);
			DrawDebugLine(GetWorld(), Ground.ImpactPoint, Ground.ImpactPoint + HorizontalVelocity.GetSafeNormal() * 100, FColor::Blue, false, -1, 0, 5);

			if (Ground.PhysMaterial.Get())
			{
				
				FString GroundPhysicalMaterial = Ground.PhysMaterial.Get()->GetName();
				GEngine->AddOnScreenDebugMessage(99,1,FColor::Yellow, GroundPhysicalMaterial);
			}
		}
		
		return FVector::DotProduct(Ground.ImpactNormal, HorizontalVelocity.GetSafeNormal());
	}
	else
	{
		return 0.f;
	}
}

#pragma endregion 

#pragma region Slide

void UDesertCharacterMovementComponent::EnterSlide()
{
	NavAgentProps.bCanCrouch = true;
	NavAgentProps.bCanJump = true;
	Velocity += Velocity.GetSafeNormal2D() * SlideEnterImpulse;

	// Friction
	EntryGroundFriction = GroundFriction;
	GroundFriction = SlideGroundFriction;

	EntryBrakingFriction = BrakingFriction;
	BrakingFriction = SlideBrakingFriction;
	
	bEntrySeparateFriction = bUseSeparateBrakingFriction;
	bUseSeparateBrakingFriction = bSlideSeparateFriction;
	
	// Acceleration
	MaxAcceleration = SlideMaxAcceleration;
	
	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, NULL);
}

void UDesertCharacterMovementComponent::ExitSlide()
{
	UE_LOG(LogTemp, Warning, TEXT("Exit Slide"));
	NavAgentProps.bCanCrouch = false;

	// Reset to Entry values
	GroundFriction = EntryGroundFriction;
	BrakingFriction = EntryBrakingFriction;
	bUseSeparateBrakingFriction = bEntrySeparateFriction;
}

bool UDesertCharacterMovementComponent::CanSlide()
{
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5f * FVector::DownVector;
	FName ProfileName = TEXT("BlockAll");
	bool bValidSurface = GetWorld()->LineTraceTestByProfile(Start, End, ProfileName, DesertSlideCharacterOwner->GetIgnoreCharacterParams());
	bool bEnoughSpeed = Velocity.SizeSquared() > (FMath::Square(MaxWalkSpeed) - 200);
	bool bWantsToSlide = bWantsToCrouch;
	
	return bValidSurface && (bEnoughSpeed || bWantsToSlide);
}

void UDesertCharacterMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	if (!CanSlide())
	{
		SetMovementMode(MOVE_Walking);
		StartNewPhysics(deltaTime, Iterations);
		return;
	}
	
	bJustTeleported = false;
	bool bCheckedFall = false;
	bool bTriedLedgeMove = false;
	float remainingTime = deltaTime;

	// Perform the move
	while ( (remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController || HasAnimRootMotion() || CurrentRootMotion.HasOverrideVelocity() || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)) )
	{
		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime -= timeTick;

		// Save current values
		UPrimitiveComponent * const OldBase = GetMovementBase();
		const FVector PreviousBaseLocation = (OldBase != NULL) ? OldBase->GetComponentLocation() : FVector::ZeroVector;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();
		const FFindFloorResult OldFloor = CurrentFloor;

		RestorePreAdditiveRootMotionVelocity();

		// Ensure velocity is horizontal.
		MaintainHorizontalGroundVelocity();
		const FVector OldVelocity = Velocity;
		Acceleration.Z = 0.f;

		FVector SlopeForce = CurrentFloor.HitResult.Normal;
		SlopeForce.Z = 0.f;

		float CrouchFactor = IsCrouching() ? SlideCrouchFactor : 1.0f;
		Velocity += SlopeForce * CrouchFactor * SlideGravityForce * deltaTime;

		Acceleration = Acceleration * SlideControllability;
		
		// Apply acceleration
		CalcSlideVelocity(timeTick, GroundFriction * SlideVelocityFrictionFactor, bSlidingFriction, GetMaxBrakingDeceleration());


		// Compute move parameters
		const FVector MoveVelocity = Velocity;
		const FVector Delta = timeTick * MoveVelocity;
		const bool bZeroDelta = Delta.IsNearlyZero();
		FStepDownResult StepDownResult;
		bool bFloorWalkable = CurrentFloor.IsWalkableFloor();

		if ( bZeroDelta )
		{
			remainingTime = 0.f;
		}
		else
		{
			// try to move forward
			MoveAlongFloor(MoveVelocity, timeTick, &StepDownResult);

			if ( IsFalling() )
			{
				// pawn decided to jump up
				const float DesiredDist = Delta.Size();
				if (DesiredDist > UE_KINDA_SMALL_NUMBER)
				{
					const float ActualDist = (UpdatedComponent->GetComponentLocation() - OldLocation).Size2D();
					remainingTime += timeTick * (1.f - FMath::Min(1.f,ActualDist/DesiredDist));
				}
				StartNewPhysics(remainingTime,Iterations);
				return;
			}
			else if ( IsSwimming() ) //just entered water
			{
				StartSwimming(OldLocation, OldVelocity, timeTick, remainingTime, Iterations);
				return;
			}
		}

		// Update floor.
		// StepUp might have already done it for us.
		if (StepDownResult.bComputedFloor)
		{
			CurrentFloor = StepDownResult.FloorResult;
		}
		else
		{
			FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, bZeroDelta, NULL);
		}

		// check for ledges here
		const bool bCheckLedges = !CanWalkOffLedges();
		if ( bCheckLedges && !CurrentFloor.IsWalkableFloor() )
		{
			// calculate possible alternate movement
			const FVector GravDir = FVector(0.f,0.f,-1.f);
			const FVector NewDelta = bTriedLedgeMove ? FVector::ZeroVector : GetLedgeMove(OldLocation, Delta, GravDir);
			if ( !NewDelta.IsZero() )
			{
				// first revert this move
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, false);

				// avoid repeated ledge moves if the first one fails
				bTriedLedgeMove = true;

				// Try new movement direction
				Velocity = NewDelta/timeTick;
				remainingTime += timeTick;
				continue;
			}
			else
			{
				// see if it is OK to jump
				// @todo collision : only thing that can be problem is that oldbase has world collision on
				bool bMustJump = bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ( (bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump) )
				{
					return;
				}
				bCheckedFall = true;

				// revert this move
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, true);
				remainingTime = 0.f;
				break;
			}
		}
		else
		{
			// Validate the floor check
			if (CurrentFloor.IsWalkableFloor())
			{
				if (ShouldCatchAir(OldFloor, CurrentFloor))
				{
					HandleWalkingOffLedge(OldFloor.HitResult.ImpactNormal, OldFloor.HitResult.Normal, OldLocation, timeTick);
					if (IsMovingOnGround())
					{
						// If still walking, then fall. If not, assume the user set a different mode they want to keep.
						StartFalling(Iterations, remainingTime, timeTick, Delta, OldLocation);
					}
					return;
				}

				AdjustFloorHeight();
				SetBase(CurrentFloor.HitResult.Component.Get(), CurrentFloor.HitResult.BoneName);
			}
			else if (CurrentFloor.HitResult.bStartPenetrating && remainingTime <= 0.f)
			{
				// The floor check failed because it started in penetration
				// We do not want to try to move downward because the downward sweep failed, rather we'd like to try to pop out of the floor.
				FHitResult Hit(CurrentFloor.HitResult);
				Hit.TraceEnd = Hit.TraceStart + FVector(0.f, 0.f, MAX_FLOOR_DIST);
				const FVector RequestedAdjustment = GetPenetrationAdjustment(Hit);
				ResolvePenetration(RequestedAdjustment, Hit, UpdatedComponent->GetComponentQuat());
				bForceNextFloorCheck = true;
			}

			// check if just entered water
			if ( IsSwimming() )
			{
				StartSwimming(OldLocation, Velocity, timeTick, remainingTime, Iterations);
				return;
			}

			// See if we need to start falling.
			if (!CurrentFloor.IsWalkableFloor() && !CurrentFloor.HitResult.bStartPenetrating)
			{
				const bool bMustJump = bJustTeleported || bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ((bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump) )
				{
					return;
				}
				bCheckedFall = true;
			}
		}


		// Allow overlap events and such to change physics state and velocity
		if (IsMovingOnGround() && bFloorWalkable)
		{
			// Make velocity reflect actual move
			if( !bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && timeTick >= MIN_TICK_TIME)
			{
				// TODO-RootMotionSource: Allow this to happen during partial override Velocity, but only set allowed axes?
				Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / timeTick;
				MaintainHorizontalGroundVelocity();
			}
		}

		// If we didn't move at all this iteration then abort (since future iterations will also be stuck).
		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			remainingTime = 0.f;
			break;
		}	
	}

	FHitResult Hit;
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(Velocity.GetSafeNormal2D(), FVector::UpVector).ToQuat();
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, false, Hit);
}

bool UDesertCharacterMovementComponent::GetSlideSurface(FHitResult& Hit) const
{
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5f * FVector::DownVector;
	
	FName ProfileName = TEXT("BlockAll");
	
	FCollisionQueryParams Params = DesertSlideCharacterOwner->GetIgnoreCharacterParams();
	Params.bReturnPhysicalMaterial = true;
	
	return GetWorld()->LineTraceSingleByProfile(Hit, Start, End, ProfileName, Params);
}

#pragma endregion 

#pragma region Input

void UDesertCharacterMovementComponent::SprintPressed()
{
	Safe_bWantsToSprint = true;
}

void UDesertCharacterMovementComponent::SprintReleased()
{
	Safe_bWantsToSprint = false;
}

void UDesertCharacterMovementComponent::CrouchPressed()
{
	bWantsToCrouch = true;
}

void UDesertCharacterMovementComponent::CrouchReleased()
{
	bWantsToCrouch = false;
}

bool UDesertCharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}

bool UDesertCharacterMovementComponent::IsMovementMode(EMovementMode InMovementMode) const
{
	return InMovementMode == MovementMode;
}

#pragma endregion
