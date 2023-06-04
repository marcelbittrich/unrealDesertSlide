// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/WidgetSwitcher.h"
#include "UObject/Class.h"
#include "GameFramework/GameUserSettings.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;
	
	if (!SoloButton) return false;
	SoloButton->OnClicked.AddDynamic(this, &UMainMenu::StartSoloGame);

	if (!OptionsButton) return false;
	OptionsButton->OnClicked.AddDynamic(this, &UMainMenu::OpenOptionsMenu);
	
	if (!QuitGameButton) return false;
	QuitGameButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	
	if (!ExitButton) return false;
	ExitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	if (!ApplySettingsButton) return false;
	ApplySettingsButton->OnClicked.AddDynamic(this, &UMainMenu::ApplySettings);
	
	return true;
}

void UMainMenu::StartSoloGame()
{
	if (MenuInterface)
	{
		MenuInterface->LoadSoloLevel();
	}
}

void UMainMenu::OpenOptionsMenu()
{
	ReadCurrentSettings();

	if (MenuSwitcher && Options)
	{
		MenuSwitcher->SetActiveWidget(Options);
	}
}

void UMainMenu::ReadCurrentSettings()
{
	if (!GameUserSettings) return;

	UE_LOG(LogTemp, Warning, TEXT("Config Dir: %s"), *GGameUserSettingsIni);
	
	GetCurrentWindowMode();
	GetCurrentResolution();
	GetCurrentFrameRateLimit();
	GetCurrentQualityOptions();
}

void UMainMenu::ApplySettings()
{
	if (MenuSwitcher && Selector)
	{
		MenuSwitcher->SetActiveWidget(Selector);
	}

	if (!GameUserSettings)
	{
		return;
	}
	
	SetWindowMode();
	SetResolution();
	SetFrameRateLimit();
	SetQualityOptions();
	
	GameUserSettings->ApplySettings(false);
}

void UMainMenu::QuitGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Quit Game"));
	if (MenuInterface)
	{
		MenuInterface->QuitGame();
	}
}
