// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaceWidget.h"
#include "Timings.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API UTimings : public URaceWidget
{
	GENERATED_BODY()
	
public:
	void SetLapTimeText(float Time);
	void SetLastLapTimeText(float Time);
	void SetCurrentLapText(int CurrentLap, int MaxLaps);
	void SetPersonalBestText(float Time);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LapTimeText;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LastLapTimeText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentLapText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PersonalBestText;
};
