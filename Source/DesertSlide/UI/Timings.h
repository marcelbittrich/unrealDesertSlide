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
	void SetGameTime(float GameTime);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GameTime;

	float GameTimeValue;
};
