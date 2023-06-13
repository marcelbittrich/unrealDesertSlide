// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertSlidePlayerState.h"

#include "Net/UnrealNetwork.h"

void ADesertSlidePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ADesertSlidePlayerState, bReadyToRace, COND_SkipOwner);
}

void ADesertSlidePlayerState::SetReadyToRace(bool bReady)
{
	if(HasAuthority())
	{
		bReadyToRace = bReady;
	}
}
