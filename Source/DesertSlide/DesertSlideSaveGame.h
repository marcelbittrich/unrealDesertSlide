// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DesertSlideSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API UDesertSlideSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	float PersonalBestTime;
};
