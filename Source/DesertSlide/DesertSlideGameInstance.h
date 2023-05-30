// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuInterface.h"
#include "Engine/GameInstance.h"
#include "DesertSlideGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API UDesertSlideGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

	UDesertSlideGameInstance(const FObjectInitializer & ObjectInitializer);
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();
	
	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();
	
public:
	UFUNCTION()
	virtual void LoadSoloLevel() override;

	UFUNCTION()
	virtual void GoToMainMenu() override;
	
	UFUNCTION()
	virtual void QuitGame() override;
	
	UFUNCTION()
	virtual void RestartLevel() override;

	UFUNCTION()
	virtual void StartRace() override;

private:

	UPROPERTY()
	TSubclassOf<class UUserWidget> MainMenuClass;
	UPROPERTY()
	class UMenuWidget* MainMenu;
	
	UPROPERTY()
	TSubclassOf<class UUserWidget> InGameMenuClass;
	UPROPERTY()
	class UMenuWidget* InGameMenu;
};
