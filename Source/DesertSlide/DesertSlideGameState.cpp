// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertSlideGameState.h"

#include "DesertSlidePlayerState.h"


void ADesertSlideGameState::ChangePlayerReady(APlayerController* PlayerController, bool bReady)
{
	if (!PlayerController) return;
	
	ADesertSlidePlayerState* PlayerState = PlayerController->GetPlayerState<ADesertSlidePlayerState>();
	if (!PlayerState) return;

	PlayerState->bReadyToRace = bReady;

	if (bReady)
	{
		UE_LOG(LogTemp,Warning, TEXT("Player Ready"));
	}
	else
	{
		
		UE_LOG(LogTemp,Warning, TEXT("Player not Ready"));
	}

	
	CheckAllPlayerReady();
}

void ADesertSlideGameState::CheckAllPlayerReady()
{
	if (!HasAuthority()) return;
	
	for (APlayerState* Player : PlayerArray)
	{
		ADesertSlidePlayerState* DesertSlidePlayer = Cast<ADesertSlidePlayerState>(Player);
		if (!DesertSlidePlayer) break;

		bool bReady = DesertSlidePlayer->GetReadyToRace();
		FString PlayerName = DesertSlidePlayer->GetPlayerName();
		
		if (bReady)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player %s is ready"), *PlayerName);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player %s is NOT ready"), *PlayerName);
		}
	}
}
