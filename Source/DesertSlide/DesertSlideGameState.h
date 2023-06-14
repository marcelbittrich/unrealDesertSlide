// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DesertSlideGameState.generated.h"

/**
 * 
 */

class ADesertSlidePlayerState;

USTRUCT()
struct FPlayerData
{
	GENERATED_BODY()
	UPROPERTY()
	FUniqueNetIdRepl UniqueID;
	UPROPERTY()
	FString Name;
	UPROPERTY()
	bool ReadyToRace;
};

UCLASS()
class DESERTSLIDE_API ADesertSlideGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetPlayerReady(APlayerState* PlayerState, bool bReady);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSendPlayerStates(const TArray<FPlayerData>& PlayerReadinessStates);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	TArray<FPlayerData> AllPlayerReadinessStates;
	
private:
	void ChangePlayerData(ADesertSlidePlayerState* DesertSlidePlayerState, bool bReady);
	void AddNewPlayerData(ADesertSlidePlayerState* DesertSlidePlayerState);
	
public:
	void RemovePlayerData(AController* Exiting);
	void CheckAllPlayersReadyState();
};
