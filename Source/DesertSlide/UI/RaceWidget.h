// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RaceWidget.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API URaceWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void Setup();
	virtual void Teardown();
};
