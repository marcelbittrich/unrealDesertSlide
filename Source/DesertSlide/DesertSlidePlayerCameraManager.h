// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "DesertSlidePlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API ADesertSlidePlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float CrouchBlendDuration = 0.5;
	float CrouchBlendTime = 0;	

public:
	ADesertSlidePlayerCameraManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
};
