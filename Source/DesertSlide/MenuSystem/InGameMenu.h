// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* StartRaceInGameMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* RestartInGameMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* CancelInGameMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* LeaveGameInGameMenuButton;

	//UPROPERTY(meta = (BindWidget))
	//class UCheckBox* ReadyToRaceCheckbox;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* FollowCamCheckBox;

	UFUNCTION(BlueprintCallable)
	void StartRace();

	UFUNCTION()
	void HandleReadyToRaceChange(bool bReady);

	UFUNCTION()
	void HandleFollowCamChange(bool bFollowCam);
	
	UFUNCTION(BlueprintCallable)
	void Restart();
	UFUNCTION(BlueprintCallable)
	void Cancel();
	UFUNCTION()
	void LeaveGame();
};
