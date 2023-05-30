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
	
	FrameRateSelect->ClearOptions();
	for (EFrameRateLimit FrameRateLimit : TEnumRange<EFrameRateLimit>())
	{
		FString FrameRateLimitString = UEnum::GetDisplayValueAsText(FrameRateLimit).ToString();
		FrameRateSelect->AddOption(FrameRateLimitString);
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
	
	GetWindowMode();
	GetResolution();
	GetFrameRateLimit();

	GetQualitySetting(EQualitySelector::Shadow, ShadowQualitySelect);
	GetQualitySetting(EQualitySelector::Texture, TextureQualitySelect);
	GetQualitySetting(EQualitySelector::AntiAliasing, AntiAliasingSelect);
	GetQualitySetting(EQualitySelector::PostProcess, PostProcessSelect);
	GetQualitySetting(EQualitySelector::ViewDistance, ViewDistanceSelect);
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
	
	SetQualitySetting(EQualitySelector::Shadow, ShadowQualitySelect->GetSelectedIndex());
	SetQualitySetting(EQualitySelector::Texture, TextureQualitySelect->GetSelectedIndex());
	SetQualitySetting(EQualitySelector::AntiAliasing, AntiAliasingSelect->GetSelectedIndex());
	SetQualitySetting(EQualitySelector::PostProcess, PostProcessSelect->GetSelectedIndex());
	SetQualitySetting(EQualitySelector::ViewDistance, ViewDistanceSelect->GetSelectedIndex());
	
	GameUserSettings->ApplySettings(false);
}

void UMainMenu::GetWindowMode()
{
	FString CurrentWindowMode = UEnum::GetDisplayValueAsText(GameUserSettings->GetFullscreenMode()).ToString();
	WindowModeSelect->SetSelectedOption(CurrentWindowMode);
}

void UMainMenu::SetWindowMode()
{
	int WindowModeIndex = WindowModeSelect->GetSelectedIndex();
	switch (WindowModeIndex)
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

void UMainMenu::GetResolution()
{
	FIntPoint CurrentResolution = GameUserSettings->GetScreenResolution();
	FString CurrentResolutionString = FString::FromInt(CurrentResolution.X) + " x " + FString::FromInt(CurrentResolution.Y);
	ResolutionSelect->SetSelectedOption(CurrentResolutionString);
	UE_LOG(LogTemp, Warning, TEXT("Get Resolution to %s"), *CurrentResolutionString);
}

void UMainMenu::SetResolution()
{
	int ResolutionIndex = ResolutionSelect->GetSelectedIndex();
	FIntPoint ResolutionSize;
	switch (ResolutionIndex)
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
	UE_LOG(LogTemp, Warning, TEXT("Set Resolution to %s"), *ResolutionSize.ToString());
}

void UMainMenu::GetFrameRateLimit()
{
	float CurrentFrameRateLimit = GameUserSettings->GetFrameRateLimit();
	FString CurrentFrameRateLimitString;
	if (CurrentFrameRateLimit != 0)
	{
		CurrentFrameRateLimitString = FString::SanitizeFloat(CurrentFrameRateLimit, 0) + " FPS";
	}
	else
	{
		CurrentFrameRateLimitString = "Unlimited";
	}
	FrameRateSelect->SetSelectedOption(CurrentFrameRateLimitString);
	UE_LOG(LogTemp, Warning, TEXT("Get FrameRateLimit to %s"), *CurrentFrameRateLimitString);
}

void UMainMenu::SetFrameRateLimit()
{
	int FrameRateLimitIndex = FrameRateSelect->GetSelectedIndex();
	float FrameRateLimit;
	switch (FrameRateLimitIndex)
	{
	case 0:
		FrameRateLimit = 30;
		break;
	case 1:
		FrameRateLimit = 60;
		break;
	case 2:
		FrameRateLimit = 120;
		break;
	case 3:
		FrameRateLimit = 0;
		break;
	default:
		FrameRateLimit = 0;
		break;
	}
	GameUserSettings->SetFrameRateLimit(FrameRateLimit);
	UE_LOG(LogTemp, Warning, TEXT("Set FrameRateLimit to %f"), FrameRateLimit);
}

void UMainMenu::GetQualitySetting(EQualitySelector QualitySelector, UComboBoxString* ComboBox)
{
	int CurrentQualitySetting = 1;
	switch (QualitySelector)
	{
	case EQualitySelector::Shadow:
		CurrentQualitySetting = GameUserSettings->GetShadowQuality();
		break;
	case EQualitySelector::Texture:
		CurrentQualitySetting = GameUserSettings->GetTextureQuality();
		break;
	case EQualitySelector::AntiAliasing:
		CurrentQualitySetting = GameUserSettings->GetPostProcessingQuality();
		break;
	case EQualitySelector::PostProcess:
		CurrentQualitySetting = GameUserSettings->GetPostProcessingQuality();
		break;
	case EQualitySelector::ViewDistance:
		CurrentQualitySetting = GameUserSettings->GetViewDistanceQuality();
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Requested QulitySelector not found in GetQualitySettings"));
		break;
	}

	switch (CurrentQualitySetting)
	{
	case 0:
		ComboBox->SetSelectedOption("Low");
		break;
	case 1:
		ComboBox->SetSelectedOption("Medium");
		break;
	case 2:
		ComboBox->SetSelectedOption("High");
		break;
	case 3:
		ComboBox->SetSelectedOption("Epic");
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Requested CurrentQualitySetting not found in GetQualitySettings"));
		break;
	}
	UE_LOG(LogTemp, Warning, TEXT("Get %s to %d"), *UEnum::GetDisplayValueAsText(QualitySelector).ToString(), CurrentQualitySetting);
}

void UMainMenu::SetQualitySetting(EQualitySelector QualitySelector, int QualitySettingIndex)
{
	int QualitySetting = 1;
	switch (QualitySettingIndex)
	{
	case 0:
		QualitySetting = 0;
		break;
	case 1:
		QualitySetting = 1;
		break;
	case 2:
		QualitySetting = 2;
		break;
	case 3:
		QualitySetting = 3;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Requested QulitySetting not found in SetQualitySettings"));
		break;
	}

	switch (QualitySelector)
	{
	case EQualitySelector::Shadow:
		GameUserSettings->SetShadowQuality(QualitySetting);
		break;
	case EQualitySelector::Texture:
		GameUserSettings->SetTextureQuality(QualitySetting);
		break;
	case EQualitySelector::AntiAliasing:
		GameUserSettings->SetPostProcessingQuality(QualitySetting);
		break;
	case EQualitySelector::PostProcess:
		GameUserSettings->SetPostProcessingQuality(QualitySetting);
		break;
	case EQualitySelector::ViewDistance:
		GameUserSettings->SetViewDistanceQuality(QualitySetting);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Requested QulitySelector not found in SetQualitySettings"));
		break;
	}
	UE_LOG(LogTemp, Warning, TEXT("Set %s to %d"), *UEnum::GetDisplayValueAsText(QualitySelector).ToString(), QualitySetting);
}

void UMainMenu::QuitGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Quit Game"));
	if (MenuInterface)
	{
		MenuInterface->QuitGame();
	}
}
