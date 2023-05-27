// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/WidgetSwitcher.h"
#include "UObject/Class.h"
#include "GameFramework/GameUserSettings.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{

}

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

	GameUserSettings = GEngine->GetGameUserSettings();

	WindowModeSelect->ClearOptions();
	for (EScreenMode ScreenMode : TEnumRange<EScreenMode>())
	{
		FString ScreenModeString = UEnum::GetDisplayValueAsText(ScreenMode).ToString();
		WindowModeSelect->AddOption(ScreenModeString);
	}

	ResolutionSelect->ClearOptions();
	for (EResolution Resolution : TEnumRange<EResolution>())
	{
		FString ResolutionString = UEnum::GetDisplayValueAsText(Resolution).ToString();
		ResolutionSelect->AddOption(ResolutionString);
	}
	
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
	ReadOptions();

	if (MenuSwitcher && Options)
	{
		MenuSwitcher->SetActiveWidget(Options);
	}
}

void UMainMenu::ReadOptions()
{
	if (!GameUserSettings) return;
	
	FString CurrentWindowMode = UEnum::GetDisplayValueAsText(GameUserSettings->GetFullscreenMode()).ToString();
	WindowModeSelect->SetSelectedOption(CurrentWindowMode);
	
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
	

	int32 ShadowQuality = GameUserSettings->GetShadingQuality();
	UE_LOG(LogTemp, Warning, TEXT("ShadowQuality: %d"), ShadowQuality);
	
	SetWindowMode();
	SetResolution();
	
	GameUserSettings->ApplySettings(false);
}

void UMainMenu::SetWindowMode()
{
	int WindowMode = WindowModeSelect->GetSelectedIndex();
	switch (WindowMode)
	{
	case 0:
		GameUserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
		break;
	case 1:
		GameUserSettings->SetFullscreenMode(EWindowMode::Windowed);
		break;
	default:
		break;
	}
}

void UMainMenu::SetResolution()
{
	int Resolution = ResolutionSelect->GetSelectedIndex();
	FIntPoint ResolutionSize;
	switch (Resolution)
	{
	case 0:
		ResolutionSize.X = 1280;
		ResolutionSize.Y = 720;
		break;
	case 1:
		ResolutionSize.X = 1600;
		ResolutionSize.Y = 900;
		break;
	case 2:
		ResolutionSize.X = 1920;
		ResolutionSize.Y = 1080;
		break;
	case 3:
		ResolutionSize.X = 2560;
		ResolutionSize.Y = 1440;
		break;
	case 4:
		ResolutionSize.X = 3820;
		ResolutionSize.Y = 2160;
		break;
	default:
		ResolutionSize.X = 1280;
		ResolutionSize.Y = 720;
		break;
	}
	GameUserSettings->SetScreenResolution(ResolutionSize);
}

void UMainMenu::QuitGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Quit Game"));
	if (MenuInterface)
	{
		MenuInterface->QuitGame();
	}
}
