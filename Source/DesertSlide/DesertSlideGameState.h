// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DesertSlideGameState.generated.h"

/**
 * 
 */

class ADesertSlidePlayerState;

UCLASS()
class DESERTSLIDE_API ADesertSlideGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetPlayerReady(APlayerState* PlayerState, bool bReady);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const override;
	
private:
	void ChangePlayerData(ADesertSlidePlayerState* DesertSlidePlayerState, bool bReady);
	
public:
	void CheckAllPlayersReadyState();
};
