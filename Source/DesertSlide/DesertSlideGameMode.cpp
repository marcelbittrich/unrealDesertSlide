// Copyright Epic Games, Inc. All Rights Reserved.

#include "DesertSlideGameMode.h"
#include "DesertSlideGameState.h"
#include "UObject/ConstructorHelpers.h"

ADesertSlideGameMode::ADesertSlideGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/DesertSlide/Characters/BP_DesertSlideCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Default Pawn for GameMode not found!"));
	}
}

void ADesertSlideGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("New Login"));

	CheckAllPlayersReadyState();
}

void ADesertSlideGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	UE_LOG(LogTemp, Warning, TEXT("Logout"));

	if (ADesertSlideGameState* DesertGameState = GetGameState<ADesertSlideGameState>())
	{
		DesertGameState->RemovePlayerData(Exiting);
	}
}

void ADesertSlideGameMode::CheckAllPlayersReadyState()
{
	ADesertSlideGameState* DesertGameState = GetGameState<ADesertSlideGameState>();
	if (!DesertGameState) return;
	
	DesertGameState->CheckAllPlayersReadyState();
}
