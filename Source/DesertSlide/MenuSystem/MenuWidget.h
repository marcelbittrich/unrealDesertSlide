// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* Interface);
	void Setup();
	UFUNCTION(BlueprintCallable)
	void Teardown();
	
protected:
	IMenuInterface* MenuInterface;
};
