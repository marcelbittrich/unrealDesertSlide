// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DesertSlidePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API ADesertSlidePlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TArray<struct FPlayerData> LocalPlayerReadinessStates;
	
public:
	void SetPlayerEnabledState(bool bPlayerEnabled);

	virtual void Tick(float DeltaSeconds) override;

	void AutoRotateCamera(float DeltaSeconds);

	UPROPERTY()
	bool bAutoRotate = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bFollowCam = false;
	UPROPERTY(EditAnywhere)
	float AutoRotatePitchAngle = 10.f;
	UPROPERTY(EditAnywhere)
	float AutoRotateMinVelocity = 200;
	UPROPERTY(EditAnywhere)//, meta  = (ClampMin = "0", ClampMax = "1.0", UIMin = "0", UIMax = "1.0"))
	float AutoRotateInterpSpeed = 0.9f;

	UFUNCTION(BlueprintCallable)
	void FlipReadyToRace();

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_SetReadyToRace();

	UFUNCTION(Client, Reliable)
	void ClientReceivePlayerStates(const TArray<struct FPlayerData>& PlayerReadinessStates);
};
