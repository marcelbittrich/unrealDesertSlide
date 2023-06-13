// Copyright Epic Games, Inc. All Rights Reserved.

#include "DesertSlideGameMode.h"
#include "DesertSlideCharacter.h"
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
