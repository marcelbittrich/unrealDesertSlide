// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CheckpointWidget.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API UCheckpointWidget : public UUserWidget
{
	GENERATED_BODY()
		
public:
	void Display(float SectorTime, float DisplayTime = 1);

private:
	void Setup();
	void Teardown();
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SectorTimeText;
};
