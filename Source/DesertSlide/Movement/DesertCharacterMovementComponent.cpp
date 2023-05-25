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
	
	
	if (MovementMode == MOVE_Walking || MovementMode == MOVE_Custom)
	{
		// MaxWalkSpeed is used in CMOVE_Slide as exit value.
		float SlopeFactor = GetGroundSlopeFactor();
		MaxWalkSpeed = Move_BaseMaxWalkSpeed + SlopeFactor * Move_SlopeWalkSpeedOffset;
	}
	
	if (MovementMode == MOVE_Walking)
	{
		float SlopeFactor = GetGroundSlopeFactor();
		MaxAcceleration = Move_BaseAcceleration + SlopeFactor * Move_SlopeAcceleration;
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

void UDesertCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DisplayDebugMessages();
}

void UDesertCharacterMovementComponent::DisplayDebugMessages()
{
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
	if ((MovementMode == MOVE_Walking) && Velocity.SizeSquared() > FMath::Square(Slide_EnterSpeed))
	{
		FHitResult PotentialSlideSurface;
		if (GetSlideSurface(PotentialSlideSurface))
		{
			UE_LOG(LogTemp, Warning, TEXT("Enter Slide"));
			EnterSlide();
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

float UDesertCharacterMovementComponent::GetGroundSlopeFactor()
{
	FHitResult Ground;
	if (GetSlideSurface(Ground))
	{
		FVector HorizontalVelocity = Velocity;
		HorizontalVelocity.Z = 0;
		
		DrawDebugLine(GetWorld(), Ground.ImpactPoint, Ground.ImpactPoint + Ground.ImpactNormal * 100, FColor::Green, false, -1, 0, 5);
		DrawDebugLine(GetWorld(), Ground.ImpactPoint, Ground.ImpactPoint + HorizontalVelocity.GetSafeNormal() * 100, FColor::Blue, false, -1, 0, 5);

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
	Velocity += Velocity.GetSafeNormal2D() * Slide_EnterImpulse;
	EntryGroundFriction = GroundFriction;
	GroundFriction = Slide_GroundFriction;
	//MaxCustomMovementSpeed = 8000;
	SetMovementMode(MOVE_Custom, CMOVE_Slide);
}

void UDesertCharacterMovementComponent::ExitSlide()
{
	UE_LOG(LogTemp, Warning, TEXT("Exit Slide"));
	NavAgentProps.bCanCrouch = false;

	// straighten character after slide
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(), FVector::UpVector).ToQuat();
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, true, Hit);
	GroundFriction = EntryGroundFriction;
	SetMovementMode(MOVE_Walking);
}

void UDesertCharacterMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	UE_LOG(LogTemp, Warning, TEXT("PhysSlide: Velocity in: %f"), Velocity.Size());
	
	if (deltaTime < MIN_TICK_TIME)
	{
		UE_LOG(LogTemp, Warning, TEXT("PhysSlide: MIN_TICK exit"));
		return;
	}

	RestorePreAdditiveRootMotionVelocity();

	FHitResult SurfaceHit;
	if(!GetSlideSurface(SurfaceHit))
	{
		UE_LOG(LogTemp, Warning, TEXT("PhysSlide: No Surface hit"));
		ExitSlide();
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

	// Surface Gravity
	float CrouchMultiplier = IsCrouching() ? 3.0f : 1.0f;
	Velocity += Slide_GravityForce * CrouchMultiplier * FVector::DownVector * deltaTime;
	
	// Acceleration
	Acceleration = Acceleration * Slide_Controllability;

	UE_LOG(LogTemp, Warning, TEXT("PhysSlide: Velocity before: %f"), Velocity.Size());
	
	// Calc Velocity
	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		// Continuous friction only functions in fluids, TODO: check function
		CalcVelocity(deltaTime, Slide_Friction, true, GetMaxBrakingDeceleration());
	}

	UE_LOG(LogTemp, Warning, TEXT("PhysSlide: Velocity middle: %f"), Velocity.Size());
	
	ApplyRootMotionToVelocity(deltaTime);

	// Perform Move
	Iterations++;
	bJustTeleported = false;

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	FQuat OldRoation = UpdatedComponent->GetComponentRotation().Quaternion();
	FHitResult Hit(1.f);
	FVector Adjusted = Velocity * deltaTime;
	FVector VelPlaneDir = FVector::VectorPlaneProject(Velocity, SurfaceHit.Normal).GetSafeNormal();

	// Either Sloped Character Position
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(VelPlaneDir, SurfaceHit.Normal).ToQuat();
	// ... or Z-aligned Character Position
	// FQuat NewRotation = FRotationMatrix::MakeFromXZ(Velocity.GetSafeNormal2D(), FVector::UpVector).ToQuat();
	
	SafeMoveUpdatedComponent(Adjusted, NewRotation, true, Hit);
	
	UE_LOG(LogTemp, Warning, TEXT("PhysSlide: Velocity after safeupdate: %f"), Velocity.Size());

	FVector NewLocation = UpdatedComponent->GetComponentLocation();
	FVector End = NewLocation + Velocity;
	DrawDebugLine(GetWorld(), NewLocation, End, FColor::Red, false, -1, 0, 10);

	if (Hit.Time < 1.f)
	{
		HandleImpact(Hit, deltaTime, Adjusted);
		
		float SurfaceSlideOutput = SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
		UE_LOG(LogTemp, Warning, TEXT("Hit, %f"), SurfaceSlideOutput);
	}

	UE_LOG(LogTemp, Warning, TEXT("PhysSlide: Velocity after calc: %f"), Velocity.Size());
	
	FHitResult NewSurfaceHit;
	if (!GetSlideSurface(NewSurfaceHit) || (Velocity.SizeSquared() < (FMath::Square(MaxWalkSpeed) - 200) && !IsCrouching()))
	{
		UE_LOG(LogTemp, Warning, TEXT("PhysSlide: MaxWalkSpeed is higher or no Surface hit after calculations"));
		UE_LOG(LogTemp, Warning, TEXT("PhysSlide: Slope: %f MaxWalkSpeed: %f, Velocity: %f"), GetGroundSlopeFactor(), MaxWalkSpeed, Velocity.Size());
		ExitSlide();
	}

	// Update Outgoing Velocity & Acceleration
	
	if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("PhysSlide: Velocity out: %f"), Velocity.Size());
}

bool UDesertCharacterMovementComponent::GetSlideSurface(FHitResult& Hit) const
{
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5f * FVector::DownVector;
	FName ProfileName = TEXT("BlockAll");
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

#pragma endregion 
