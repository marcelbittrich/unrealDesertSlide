// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DesertSlideGameMode.generated.h"

USTRUCT()
struct FDesertPlayer
{
	GENERATED_BODY()
};

UCLASS(minimalapi)
class ADesertSlideGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADesertSlideGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	
	UFUNCTION(BlueprintCallable)
	void CheckAllPlayersReadyState();

};



