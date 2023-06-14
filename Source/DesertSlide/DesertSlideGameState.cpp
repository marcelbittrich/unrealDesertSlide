// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertSlideGameState.h"

#include "DesertSlidePlayerController.h"
#include "DesertSlidePlayerState.h"
#include "Net/UnrealNetwork.h"

void ADesertSlideGameState::SetPlayerReady(APlayerState* PlayerState, bool bReady)
{
	ADesertSlidePlayerState* DesertSlidePlayerState = Cast<ADesertSlidePlayerState>(PlayerState);
	if (!DesertSlidePlayerState) return;

	DesertSlidePlayerState->bReadyToRace = bReady;
	ChangePlayerData(DesertSlidePlayerState, bReady);
	MulticastSendPlayerStates(AllPlayerReadinessStates);
	UE_LOG(LogTemp, Warning, TEXT("Set Player Ready"));

}

void ADesertSlideGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADesertSlideGameState, AllPlayerReadinessStates);
}

void ADesertSlideGameState::ChangePlayerData(ADesertSlidePlayerState* DesertSlidePlayerState, bool bReady)
{
	bool bNewPlayer = true;
	const FUniqueNetIdRepl PlayerUniqueID = DesertSlidePlayerState->GetUniqueId();
	UE_LOG(LogTemp, Warning, TEXT("GameState: AllPlayerData size: %d"), AllPlayerReadinessStates.Num());
	for (FPlayerData& PlayerData : AllPlayerReadinessStates)
	{
		if(PlayerData.UniqueID == PlayerUniqueID)
		{
			PlayerData.ReadyToRace = bReady;
			bNewPlayer = false;
			break;
		}
	}
	
	if (bNewPlayer)
	{
		AddNewPlayerData(DesertSlidePlayerState);
		UE_LOG(LogTemp, Warning, TEXT("Tried adding new player data"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Changed old player data"));
	}
}

void ADesertSlideGameState::AddNewPlayerData(ADesertSlidePlayerState* DesertSlidePlayerState)
{
	FPlayerData PlayerData;
	PlayerData.Name = DesertSlidePlayerState->GetOwner()->GetName();
	PlayerData.UniqueID = DesertSlidePlayerState->GetUniqueId();
	PlayerData.ReadyToRace = DesertSlidePlayerState->bReadyToRace;

	AllPlayerReadinessStates.Add(PlayerData);
}

void ADesertSlideGameState::RemovePlayerData(AController* Exiting)
{
	ADesertSlidePlayerState* PlayerState = Exiting->GetPlayerState<ADesertSlidePlayerState>();
	if (PlayerState)
	{
		FUniqueNetIdRepl IdToRemove = PlayerState->GetUniqueId();
		AllPlayerReadinessStates.RemoveAll([IdToRemove](const FPlayerData& PlayerData)
		{
			return PlayerData.UniqueID == IdToRemove;
		});
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not delete player data in GameState!"))
	}
}

void ADesertSlideGameState::CheckAllPlayersReadyState()
{
	UE_LOG(LogTemp, Warning, TEXT("GameMode: AllPlayerData size: %d"), AllPlayerReadinessStates.Num());
	
	for (FPlayerData& PlayerData : AllPlayerReadinessStates)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game State: %s is %s"), *PlayerData.Name, PlayerData.ReadyToRace ? *FString("ready") : *FString("not ready"));
	}
}

void ADesertSlideGameState::MulticastSendPlayerStates_Implementation(const TArray<FPlayerData>& PlayerReadinessStates)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ADesertSlidePlayerController* PlayerController = Cast<ADesertSlidePlayerController>(*It);
		if (!PlayerController) break;

		PlayerController->ClientReceivePlayerStates(PlayerReadinessStates);
	}
}
