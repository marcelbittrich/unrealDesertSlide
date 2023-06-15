// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertSlideGameState.h"

#include "DesertSlidePlayerState.h"


void ADesertSlideGameState::SetPlayerReady(APlayerState* PlayerState, bool bReady)
{
	ADesertSlidePlayerState* DesertSlidePlayerState = Cast<ADesertSlidePlayerState>(PlayerState);
	if (!DesertSlidePlayerState) return;

	DesertSlidePlayerState->bReadyToRace = bReady;
	ChangePlayerData(DesertSlidePlayerState, bReady);
	UE_LOG(LogTemp, Warning, TEXT("Set Player Ready"));
}

void ADesertSlideGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ADesertSlideGameState, AllPlayerReadinessStates);
}

void ADesertSlideGameState::ChangePlayerData(ADesertSlidePlayerState* DesertSlidePlayerState, bool bReady)
{
	UE_LOG(LogTemp, Warning, TEXT("GameState: AllPlayerData size: %d"), PlayerArray.Num());
	DesertSlidePlayerState->bReadyToRace = bReady;
}

void ADesertSlideGameState::CheckAllPlayersReadyState()
{
	UE_LOG(LogTemp, Warning, TEXT("GameMode: AllPlayerData size: %d"), PlayerArray.Num());
	
	for (TObjectPtr<APlayerState> PlayerData : PlayerArray)
	{
		ADesertSlidePlayerState* DesertSlidePlayerState = Cast<ADesertSlidePlayerState>(PlayerData);
		if (!DesertSlidePlayerState) continue;
		UE_LOG(LogTemp, Warning, TEXT("Game State: %s is %s"), *DesertSlidePlayerState->GetName(), DesertSlidePlayerState->bReadyToRace ? *FString("ready") : *FString("not ready"));
	}
}