// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuInterface.h"
#include "Engine/GameInstance.h"
#include "DesertSlideSaveGame.h"
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
	
public:
	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();
	
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

	UFUNCTION()
	FORCEINLINE UDesertSlideSaveGame* GetSaveGame() const {return SaveGame;}

	UFUNCTION()
	void LoadMapSaveGame();
	UFUNCTION()
	void WriteMapSaveGame();

private:
	void LoadSaveGame(const FString& MapName);
	void WriteSaveGame(const FString& MapName);

	UPROPERTY()
	TSubclassOf<class UUserWidget> MainMenuClass;
	UPROPERTY()
	class UMenuWidget* MainMenu;
	
	UPROPERTY()
	TSubclassOf<class UUserWidget> InGameMenuClass;
	UPROPERTY()
	class UMenuWidget* InGameMenu;

	UPROPERTY()
	class UDesertSlideSaveGame* SaveGame;

	int UserIndex = 1;
};
