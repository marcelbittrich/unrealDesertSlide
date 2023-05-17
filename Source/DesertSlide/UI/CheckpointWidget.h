// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaceWidget.h"
#include "CheckpointWidget.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API UCheckpointWidget : public URaceWidget
{
	GENERATED_BODY()
		
public:
	void Display(float SectorTime, float DisplayTime = 1);

private:
	// Setup and Teardown should be private on this widget.
	// Display() is only necessary.
	virtual void Setup() override;
	virtual void Teardown() override;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SectorTimeText;
};
