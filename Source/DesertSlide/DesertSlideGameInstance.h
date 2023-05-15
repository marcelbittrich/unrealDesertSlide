// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DesertSlideGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API UDesertSlideGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UDesertSlideGameInstance(const FObjectInitializer & ObjectInitializer);
	virtual void Init() override;
	
public:
	UFUNCTION()
	void CheckPointCrossed();

	UFUNCTION(BlueprintCallable)
	void AddTimingsUI();

	UFUNCTION(BlueprintCallable)
	void UpdateTimings();
	
private:
	float RaceTime; 


	UPROPERTY()
	TSubclassOf<class UUserWidget> TimingsUIClass;
	
	UPROPERTY()
	class UTimings* TimingsUI;
};
