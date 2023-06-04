// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsMenu.h"

#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

UOptionsMenu::UOptionsMenu(const FObjectInitializer& ObjectInitializer)
{
	
}

bool UOptionsMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	InitializeResolutionOptions();
	InitializeWindowModeOptions();
	InitializeFrameRateLimitOptions();
	InitializeQualityOptions();
	
	GameUserSettings = GEngine->GetGameUserSettings();
	return true;
}

#pragma region WindowMode

void UOptionsMenu::DefineWindowModeOptions()
{
	const FWindowMode Fullscreen("Fullscreen", EWindowMode::Fullscreen);
	WindowModeOptions.Add(Fullscreen);
	const FWindowMode Windowed("Windowed", EWindowMode::Windowed);
	WindowModeOptions.Add(Windowed);
}

void UOptionsMenu::InitializeWindowModeOptions()
{
	if (!WindowModeSelect) return;

	DefineWindowModeOptions();
	
	WindowModeSelect->ClearOptions();
	for(const FWindowMode& WindowModeOption : WindowModeOptions)
	{
		WindowModeSelect->AddOption(WindowModeOption.Name);
	}
}

void UOptionsMenu::GetCurrentWindowMode()
{
	const EWindowMode::Type CurrentWindowMode = GameUserSettings->GetFullscreenMode();
	FWindowMode CurrentWindowModeOption;
	for(const FWindowMode& WindowModeOption : WindowModeOptions)
	{
		if (WindowModeOption.WindowMode == CurrentWindowMode)
		{
			CurrentWindowModeOption = WindowModeOption;
			break;
		}
	}
	WindowModeSelect->SetSelectedOption(CurrentWindowModeOption.Name);
	UE_LOG(LogTemp, Warning, TEXT("Get WindowMode to %s"), *CurrentWindowModeOption.Name);
}

void UOptionsMenu::SetWindowMode()
{
	const int Index = WindowModeSelect->GetSelectedIndex();
	if (WindowModeOptions.Num() > Index && Index != -1)
	{
		GameUserSettings->SetFullscreenMode(WindowModeOptions[Index].WindowMode);
		UE_LOG(LogTemp, Warning, TEXT("Set WindowMode to %s"), *UEnum::GetDisplayValueAsText(WindowModeOptions[Index].WindowMode).ToString());
	}
}

#pragma endregion 

#pragma region Resolution

void UOptionsMenu::DefineResolutionOptions()
{
	const FResolution Low("1280 x 720", FIntPoint(1280,720));
	ResolutionOptions.Add(Low);

	const FResolution Medium("1920 x 1080", FIntPoint(1920,1080));
	ResolutionOptions.Add(Medium);

	const FResolution High("2560 x 1440", FIntPoint(2560,1440));
	ResolutionOptions.Add(High);

	const FResolution Highest("3820 x 2160", FIntPoint(3820,2160));
	ResolutionOptions.Add(Highest);
}

void UOptionsMenu::InitializeResolutionOptions()
{
	if (!ResolutionSelect) return;

	DefineResolutionOptions();
	
	ResolutionSelect->ClearOptions();
	for(const FResolution& ResolutionOption : ResolutionOptions)
	{
		ResolutionSelect->AddOption(ResolutionOption.Name);
	}
}

void UOptionsMenu::GetCurrentResolution()
{
	const FIntPoint CurrentResolution = GameUserSettings->GetScreenResolution();

	FResolution CurrentResolutionOption;
	for(const FResolution& ResolutionOption : ResolutionOptions)
	{
		if (ResolutionOption.Size == CurrentResolution)
		{
			CurrentResolutionOption = ResolutionOption;
			break;
		}
	}
	ResolutionSelect->SetSelectedOption(CurrentResolutionOption.Name);
	UE_LOG(LogTemp, Warning, TEXT("Get Resolution to %s"), *CurrentResolutionOption.Name);
}

void UOptionsMenu::SetResolution()
{
	const int Index = ResolutionSelect->GetSelectedIndex();
	if (ResolutionOptions.Num() > Index && Index != -1)
	{
		GameUserSettings->SetScreenResolution(ResolutionOptions[Index].Size);
		UE_LOG(LogTemp, Warning, TEXT("Set Resolution to %s"), *ResolutionOptions[Index].Size.ToString());
	}
}

#pragma endregion

#pragma region FrameRateLimit

void UOptionsMenu::DefineFrameRateLimitOptions()
{
	const FFrameRateLimit Low("30 FPS", 30.f);
	FrameRateLimitOptions.Add(Low);
	
	const FFrameRateLimit Medium("60 FPS", 60.f);
	FrameRateLimitOptions.Add(Medium);
	
	const FFrameRateLimit High("120 FPS", 120.f);
	FrameRateLimitOptions.Add(High);
	
	const FFrameRateLimit Unlimited("Unlimited", 0.f);
	FrameRateLimitOptions.Add(Unlimited);
}

void UOptionsMenu::InitializeFrameRateLimitOptions()
{
	if (!FrameRateSelect) return;

	DefineFrameRateLimitOptions();
	
	FrameRateSelect->ClearOptions();
	for(const FFrameRateLimit& FrameRateLimitOption : FrameRateLimitOptions)
	{
		FrameRateSelect->AddOption(FrameRateLimitOption.Name);
	}
}

void UOptionsMenu::GetCurrentFrameRateLimit()
{
	const float CurrentFrameRateLimit = GameUserSettings->GetFrameRateLimit();
	FFrameRateLimit CurrentFrameRateLimitOption;
	for(const FFrameRateLimit& FrameRateLimitOption : FrameRateLimitOptions)
	{
		if (FrameRateLimitOption.Limit == CurrentFrameRateLimit)
		{
			CurrentFrameRateLimitOption = FrameRateLimitOption;
			break;
		}
	}
	FrameRateSelect->SetSelectedOption(CurrentFrameRateLimitOption.Name);
	UE_LOG(LogTemp, Warning, TEXT("Get FrameRateLimit to %s"), *CurrentFrameRateLimitOption.Name);
}

void UOptionsMenu::SetFrameRateLimit()
{
	const int Index = FrameRateSelect->GetSelectedIndex();
	if (FrameRateLimitOptions.Num() > Index && Index != -1)
	{
		GameUserSettings->SetFrameRateLimit(FrameRateLimitOptions[Index].Limit);
		UE_LOG(LogTemp, Warning, TEXT("Set Resolution to %f"), FrameRateLimitOptions[Index].Limit);
	}
}

# pragma endregion

void UOptionsMenu::DefineQualityOptions()
{
	if (ShadowQualitySelect)
	{
		const FQualitySelector ShadowQuality(
			"Shadow Quality",
			ShadowQualitySelect,
			&UGameUserSettings::GetShadowQuality,
			&UGameUserSettings::SetShadowQuality
			);
		QualitySelectors.Add(ShadowQuality);
	}
	
	if (TextureQualitySelect)
	{
		const FQualitySelector TextureQuality(
			"Texture Quality",
			TextureQualitySelect,
			&UGameUserSettings::GetTextureQuality,
			&UGameUserSettings::SetTextureQuality
			);
		QualitySelectors.Add(TextureQuality);
	}
	
	if (AntiAliasingSelect)
	{
		const FQualitySelector AntiAliasingQuality(
			"Anti Aliasing Quality",
			AntiAliasingSelect,
			&UGameUserSettings::GetAntiAliasingQuality,
			&UGameUserSettings::SetAntiAliasingQuality
			);
		QualitySelectors.Add(AntiAliasingQuality);
	}
	
	if (PostProcessSelect)
	{
		const FQualitySelector PostProcessQuality(
			"Post Process Quality",
			PostProcessSelect,
			&UGameUserSettings::GetPostProcessingQuality,
			&UGameUserSettings::SetPostProcessingQuality
			);
		QualitySelectors.Add(PostProcessQuality);
	}
	
	if (ViewDistanceSelect)
	{
		const FQualitySelector ViewDistanceQuality(
			"View Distance Quality",
			ViewDistanceSelect,
			&UGameUserSettings::GetViewDistanceQuality,
			&UGameUserSettings::SetViewDistanceQuality
			);
		QualitySelectors.Add(ViewDistanceQuality);
	}
	
	const FQualityOption Low("Low", 0);
	QualityOptions.Add(Low);
	
	const FQualityOption Medium("Medium", 1);
	QualityOptions.Add(Medium);
	
	const FQualityOption High("High", 2);
	QualityOptions.Add(High);
	
	const FQualityOption Epic("Epic", 3);
	QualityOptions.Add(Epic);
}

void UOptionsMenu::InitializeQualityOptions()
{
	DefineQualityOptions();

	if(QualitySelectors.IsEmpty()) return;
	
	for (const FQualitySelector& QualitySelector : QualitySelectors)
	{
		QualitySelector.Selector->ClearOptions();
		for (const FQualityOption& QualityOption : QualityOptions)
		{
			QualitySelector.Selector->AddOption(QualityOption.Name);
		}
	}
}

void UOptionsMenu::GetCurrentQualityOptions()
{
	for (FQualitySelector& QualitySelector : QualitySelectors)
	{
		if (QualitySelector.GetQuality == nullptr) break;
		
		const int32 CurrentQuality = (GameUserSettings->*(QualitySelector.GetQuality))();
		
		FQualityOption CurrentQualityOption;
		for(const FQualityOption& FQualityOption : QualityOptions)
		{
			if (FQualityOption.Value == CurrentQuality)
			{
				CurrentQualityOption = FQualityOption;
				break;
			}
		}
		QualitySelector.Selector->SetSelectedOption(CurrentQualityOption.Name);
		UE_LOG(LogTemp, Warning, TEXT("Get %s to %s"), *QualitySelector.Name, *CurrentQualityOption.Name);
	}
}

void UOptionsMenu::SetQualityOptions()
{
	for (FQualitySelector& QualitySelector : QualitySelectors)
	{
		if (QualitySelector.SetQuality == nullptr) break;
		
		const int Index = QualitySelector.Selector->GetSelectedIndex();
		
		if (QualityOptions.Num() > Index && Index != -1)
		{
			int32 QualityValue = QualityOptions[Index].Value;
			(GameUserSettings->*(QualitySelector.SetQuality))(QualityValue);
			UE_LOG(LogTemp, Warning, TEXT("Set %s to %d"), *QualitySelector.Name, QualityOptions[Index].Value);
		}
	}
}

