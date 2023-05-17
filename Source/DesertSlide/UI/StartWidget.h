// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaceWidget.h"
#include "StartWidget.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API UStartWidget : public URaceWidget
{
	GENERATED_BODY()

public:
	void Display(float DisplayTime = 3);
};
