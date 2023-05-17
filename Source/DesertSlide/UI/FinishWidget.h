// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaceWidget.h"
#include "FinishWidget.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API UFinishWidget : public URaceWidget
{
	GENERATED_BODY()
	
public:
	void Display(float LapTime, int FinishedLap, bool bRaceFinished, float DisplayTime = 1);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HeadlineText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeText;

	FText GetHeadlineText(int Lap, bool bRaceFinished);
};
