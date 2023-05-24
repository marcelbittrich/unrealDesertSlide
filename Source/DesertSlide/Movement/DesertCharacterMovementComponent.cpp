// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertCharacterMovementComponent.h"

#include "DesertSlideCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Math/UnrealMathUtility.h"

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
	NavAgentProps.bCanCrouch = true;
}

void UDesertCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

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

	if (MovementMode == MOVE_Walking)
	{
		if (Safe_bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
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

void UDesertCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	if (MovementMode == MOVE_Walking && !bWantsToCrouch && Safe_bPrevWantsToCrouch)
	{
		FHitResult PotentialSlideSurface;
		if (Velocity. SizeSquared() > FMath::Square(Slide_MinSpeed) && GetSlideSurface(PotentialSlideSurface))
		{
			UE_LOG(LogTemp, Warning, TEXT("Enter Slide"));
			EnterSlide();
		}

		if (IsCustomMovementMode(CMOVE_Slide) && !bWantsToCrouch)
		{
			ExitSlide();
		}
	}
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
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

#pragma endregion 

#pragma region Slide

void UDesertCharacterMovementComponent::EnterSlide()
{
	bWantsToCrouch = true;
	Velocity += Velocity.GetSafeNormal2D() * Slide_EnterImpulse;
	SetMovementMode(MOVE_Custom, CMOVE_Slide);
}

void UDesertCharacterMovementComponent::ExitSlide()
{
	UE_LOG(LogTemp, Warning, TEXT("Exit Slide"));
	bWantsToCrouch = false;

	FQuat NewRotation = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(), FVector::UpVector).ToQuat();
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, true, Hit);
	SetMovementMode(MOVE_Walking);
}

void UDesertCharacterMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		UE_LOG(LogTemp, Warning, TEXT("PhysSlide: MIN_TICK exit"));
		return;
	}

	RestorePreAdditiveRootMotionVelocity();

	FHitResult SurfaceHit;
	
	if(!GetSlideSurface(SurfaceHit) || Velocity.SizeSquared() < FMath::Square(Slide_MinSpeed))
	{
		UE_LOG(LogTemp, Warning, TEXT("PhysSlide: MinSpeed not reached or no Surface hit"));
		ExitSlide();
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

	// Surface Gravity
	Velocity += Slide_GravityForce * FVector::DownVector * deltaTime;

	// Strafe
	if (FMath::Abs(FVector::DotProduct(Acceleration.GetSafeNormal(), UpdatedComponent->GetRightVector())) > .5)
	{
		Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetRightVector());
	}
	else
	{
		Acceleration = FVector::ZeroVector;
	}

	// Calc Velocity
	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		// Friction only functions in fluids, TODO: check function
		CalcVelocity(deltaTime, Slide_Friction, true, GetMaxBrakingDeceleration());
	}
	ApplyRootMotionToVelocity(deltaTime);

	// Perform Move
	Iterations++;
	bJustTeleported = false;

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	FQuat OldRoation = UpdatedComponent->GetComponentRotation().Quaternion();
	FHitResult Hit(1.f);
	FVector Adjusted = Velocity * deltaTime;
	FVector VelPlaneDir = FVector::VectorPlaneProject(Velocity, SurfaceHit.Normal).GetSafeNormal();
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(VelPlaneDir, SurfaceHit.Normal).ToQuat();
	
	SafeMoveUpdatedComponent(Adjusted, NewRotation, true, Hit);

	if (Hit.Time < 1.f)
	{
		HandleImpact(Hit, deltaTime, Adjusted);
		SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
	}

	UE_LOG(LogTemp, Warning, TEXT("PhysSlide: Velocity after calc: %f"), Velocity.Size());
	
	FHitResult NewSurfaceHit;
	if (!GetSlideSurface(NewSurfaceHit) || Velocity.SizeSquared() < FMath::Square(Slide_MinSpeed))
	{
		UE_LOG(LogTemp, Warning, TEXT("PhysSlide: MinSpeed not reached or no Surface hit after calculations"));
		ExitSlide();
	}

	// Update Outgoing Velocity & Acceleration
	if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
	}
}

bool UDesertCharacterMovementComponent::GetSlideSurface(FHitResult& Hit) const
{
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f * FVector::DownVector;
	FName ProfileName = TEXT("BlockAll");
	
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	
	return GetWorld()->LineTraceSingleByProfile(Hit, Start, End, ProfileName, DesertSlideCharacterOwner->GetIgnoreCharacterParams());
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
	bWantsToCrouch = !bWantsToCrouch;
}

void UDesertCharacterMovementComponent::CrouchReleased()
{
	bWantsToCrouch = false;
}

bool UDesertCharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}

#pragma endregion 
