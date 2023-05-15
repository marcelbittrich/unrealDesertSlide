// Fill out your copyright notice in the Description page of Project Settings.


#include "Timings.h"

#include <string>

#include "Components/TextBlock.h"

void UTimings::Setup()
{
	this->AddToViewport();
}

void UTimings::Teardown()
{
	this->RemoveFromParent();
}

void UTimings::SetGameTime(float Time)
{
	FString GameTimeString = "GameTime:  " + FString::SanitizeFloat(Time);
	GameTime->SetText(FText::FromString(GameTimeString));
}
