// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Timings.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API UTimings : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Setup();
	void Teardown();
	
	void SetLapTimeText(float Time);
	void SetLastLapTimeText(float Time);
	void SetCurrentLapText(int CurrentLap, int MaxLaps);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LapTimeText;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LastLapTimeText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentLapText;
};
