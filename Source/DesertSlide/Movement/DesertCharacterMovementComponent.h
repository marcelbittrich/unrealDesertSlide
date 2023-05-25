// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DesertCharacterMovementComponent.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None		UMETA(Hidden),
	CMOVE_Slide		UMETA(DisplayName = "Slide"),
	CMOVE_Max		UMETA(Hidden)
};
 
UCLASS()
class DESERTSLIDE_API UDesertCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	class FSavedMove_Desert : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

		// Flag
		uint8 Saved_bWantsToSprint:1;

		uint8 Saved_bPrevWantsToCrouch:1;

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_Desert : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Desert(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};

	// Parameters

		// Walk
		UPROPERTY(EditDefaultsOnly)
		float WalkBaseMaxSpeed = 1000;
		UPROPERTY(EditDefaultsOnly)
		float WalkSlopeSpeedOffset = 1000;
		UPROPERTY(EditDefaultsOnly)
		float WalkBaseAcceleration = 500;
		UPROPERTY(EditDefaultsOnly)
		float WalkSlopeAcceleration = 500;

		// Slide
		UPROPERTY(EditDefaultsOnly)
		float SlideControllability = 5;
		UPROPERTY(EditDefaultsOnly)
		float SlideGroundFriction = 5;
		UPROPERTY(EditDefaultsOnly)
		float SlideEnterSpeed = 1200;
		UPROPERTY(EditDefaultsOnly)
		float SlideMinSpeed = 700;
		UPROPERTY(EditDefaultsOnly)
		float SlideMaxSpeed = 700;
		UPROPERTY(EditDefaultsOnly)
		float SlideEnterImpulse = 500; //TODO: evaluate relevance
		UPROPERTY(EditDefaultsOnly)
		float SlideGravityForce = 5000;
		UPROPERTY(EditDefaultsOnly)
		float SlideCrouchFactor = 3;
		UPROPERTY(EditDefaultsOnly)
		float SlideFriction = 0.1;
		UPROPERTY(EditDefaultsOnly)
		float SlideBrakingDeceleration =1000.f;

	// Transient
	UPROPERTY(Transient)
	class ADesertSlideCharacter* DesertSlideCharacterOwner;
	
	bool Safe_bWantsToSprint;
	
	bool Safe_bPrevWantsToCrouch;
	
public:
	UDesertCharacterMovementComponent();

	virtual void InitializeComponent() override;
	
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override; // Checks if we enter slide
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;
protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	
	virtual void PhysCustom(float deltaTime, int32 Iterations) override; // Links to custom PhysSlide function
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

private:
	void EnterSlide();
	void ExitSlide();
	bool CanSlide();
	void PhysSlide (float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;
	float GetGroundSlopeFactor();

	void DisplayDebugMessages();

	float EntryGroundFriction;
	
public:

	UFUNCTION(BlueprintCallable)
	void SprintPressed();
	UFUNCTION(BlueprintCallable)
	void SprintReleased();

	UFUNCTION(BlueprintCallable)
	void CrouchPressed();
	UFUNCTION(BlueprintCallable)
	void CrouchReleased();
	
	UFUNCTION(BlueprintCallable)
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;
	UFUNCTION(BlueprintCallable)
	bool IsMovementMode(EMovementMode InMovementMode) const;
};
