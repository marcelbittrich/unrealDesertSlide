// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/MenuWidget.h"
#include "MainMenu.generated.h"


class UComboBoxString;
class UButton;
class UGameUserSettings;

#pragma region OptionEnums

UENUM()
enum class EScreenMode
{
	FullScreen	UMETA(DisplayName = "FullScreen"),
	Windowed	UMETA(DisplayName = "Windowed"),
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EScreenMode, EScreenMode::Count);

UENUM()
enum class EResolution
{
	Lower	UMETA(DisplayName = "1280 x 720"),
	Low		UMETA(DisplayName = "1600 x 900"),
	Mid		UMETA(DisplayName = "1920 x 1080"),
	Higher	UMETA(DisplayName = "2560 x 1440"),
	Highest UMETA(DisplayName = "3820 x 2160"),
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EResolution, EResolution::Count);

UENUM()
enum class EFrameRateLimit
{
	Lower	UMETA(DisplayName = "30 FPS"),
	Low		UMETA(DisplayName = "60 FPS"),
	Mid		UMETA(DisplayName = "120 FPS"),
	Highest UMETA(DisplayName = "Unlimited"),
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EFrameRateLimit, EFrameRateLimit::Count);

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
class DESERTSLIDE_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

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
	UComboBoxString* WindowModeSelect;
	void GetWindowMode();
	void SetWindowMode();
	
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ResolutionSelect;
	void GetResolution();
	void SetResolution();

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* FrameRateSelect;
	void GetFrameRateLimit();
	void SetFrameRateLimit();

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ShadowQualitySelect;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* TextureQualitySelect;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* AntiAliasingSelect;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* PostProcessSelect;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ViewDistanceSelect;
	void GetQualitySetting(EQualitySelector QualitySelector, UComboBoxString* ComboBox);
	void SetQualitySetting(EQualitySelector QualitySelector, int QualitySettingIndex);
	
	UPROPERTY(meta = (BindWidget))
	UButton* ApplySettingsButton;

	UPROPERTY()
	UGameUserSettings* GameUserSettings;

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
