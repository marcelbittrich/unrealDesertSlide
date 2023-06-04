// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OptionsMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "MainMenu.generated.h"


class UComboBoxString;
class UButton;
class UGameUserSettings;

#pragma region OptionEnums
UENUM()
enum class EQualitySetting
{
	Low,
	Medium,
	High,
	Epic,
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EQualitySetting, EQualitySetting::Count);

UENUM()
enum class EQualitySelector
{
	Shadow,
	Texture,
	AntiAliasing,
	PostProcess,
	ViewDistance,
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EQualitySelector, EQualitySelector::Count);

#pragma endregion 

UCLASS()
class DESERTSLIDE_API UMainMenu : public UOptionsMenu
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

private:

	UPROPERTY(meta = (BindWidget)) 
	class UWidgetSwitcher* MenuSwitcher;

	// Menus
	UPROPERTY(meta = (BindWidget))
	class UWidget* Selector;
	UPROPERTY(meta = (BindWidget))
	class UWidget* Options;
	
	// Selector
	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;
	UPROPERTY(meta = (BindWidget))
	UButton* SoloButton;
	UPROPERTY(meta = (BindWidget))
	UButton* MultiplayerButton;
	UPROPERTY(meta = (BindWidget))
	UButton* OptionsButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	// Options
	UPROPERTY(meta = (BindWidget))
	UButton* ApplySettingsButton;

	UFUNCTION()
	void StartSoloGame();
	
	UFUNCTION()
	void OpenOptionsMenu();
	void ReadCurrentSettings();
	
	UFUNCTION()
	void ApplySettings();
	
	UFUNCTION()
	void QuitGame();
};
