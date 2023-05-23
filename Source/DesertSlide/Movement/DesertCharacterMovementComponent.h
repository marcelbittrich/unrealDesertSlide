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
	UPROPERTY(EditDefaultsOnly)
	float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly)
	float Walk_MaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly)
	float Slide_MinSpeed = 350;
	UPROPERTY(EditDefaultsOnly)
	float Slide_EnterImpulse = 500; //TODO: evaluate relevance
	UPROPERTY(EditDefaultsOnly)
	float Slide_GravityForce = 5000;
	UPROPERTY(EditDefaultsOnly)
	float Slide_Friction = 1.3;

	// Transient
	UPROPERTY(Transient)
	class ADesertSlideCharacter* DesertSlideCharacterOwner;
	
	bool Safe_bWantsToSprint;
	
	bool Safe_bPrevWantsToCrouch;
	
public:
	UDesertCharacterMovementComponent();

	virtual void InitializeComponent() override;
	
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

private:
	void EnterSlide();
	void ExitSlide();
	void PhysSlide (float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;

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
};
