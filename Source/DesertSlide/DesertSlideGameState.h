// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DesertSlideGameState.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API ADesertSlideGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ChangePlayerReady(APlayerController* PlayerController, bool bReady);
	UFUNCTION()
	void CheckAllPlayerReady();
	
};
