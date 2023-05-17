// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertSlidePlayerController.h"

void ADesertSlidePlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
	if (bPlayerEnabled)
	{
		GetPawn()->EnableInput(this);
	}
	else
	{
		GetPawn()->DisableInput(this);
	}
}
