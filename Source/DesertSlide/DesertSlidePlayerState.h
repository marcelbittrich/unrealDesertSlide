// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DesertSlidePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API ADesertSlidePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetReadyToRace(bool bReady);
	bool GetReadyToRace() const {return bReadyToRace;}

	UPROPERTY(Replicated)
	bool bReadyToRace;	
	
private:

};
