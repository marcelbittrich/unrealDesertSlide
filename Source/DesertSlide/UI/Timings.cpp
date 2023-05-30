// Fill out your copyright notice in the Description page of Project Settings.


#include "Timings.h"

#include "Components/TextBlock.h"

void UTimings::SetLapTimeText(float Time)
{
	FString LapTimeString = "Lap Time:  " + FString::SanitizeFloat(Time, 2);
	LapTimeText->SetText(FText::FromString(LapTimeString));
}

void UTimings::SetLastLapTimeText(float Time)
{
	FString LastLapTimeString = "Last Lap:  " + FString::SanitizeFloat(Time, 2);
	LastLapTimeText->SetText(FText::FromString(LastLapTimeString));
}

void UTimings::SetCurrentLapText(int CurrentLap, int MaxLaps)
{
	FString CurrentLapString = "Lap: " + FString::FromInt(CurrentLap) + " / " +  FString::FromInt(MaxLaps);
	CurrentLapText->SetText(FText::FromString(CurrentLapString));
}
