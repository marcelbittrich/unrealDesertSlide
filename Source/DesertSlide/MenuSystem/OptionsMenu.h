// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MenuWidget.h"
#include "OptionsMenu.generated.h"

class UComboBoxString;

DECLARE_DELEGATE_OneParam(FQualitySetterDelegate, int32);
DECLARE_DELEGATE_OneParam(FQualityGetterDelegate, int32);

USTRUCT()
struct FWindowMode
{
	GENERATED_BODY()
	UPROPERTY()
	FString Name;
	UPROPERTY()
	TEnumAsByte<EWindowMode::Type> WindowMode;
	
	FWindowMode()
	{
		Name = "Default";
		WindowMode = EWindowMode::Fullscreen;
	}
	FWindowMode(const FString& Name, const EWindowMode::Type WindowMode): Name(Name), WindowMode(WindowMode)
	{
	}
};

USTRUCT()
struct FResolution
{
	GENERATED_BODY()
	UPROPERTY()
	FString Name;
	UPROPERTY()
	FIntPoint Size;

	FResolution()
	{
		Name = "Default";
		Size = FIntPoint(1920,1080);
	}
	FResolution(const FString& Name, const FIntPoint& Size) : Name(Name), Size(Size)
	{
	}
};

USTRUCT()
struct FFrameRateLimit
{
	GENERATED_BODY()
	UPROPERTY()
	FString Name;
	UPROPERTY()
	float Limit;

	FFrameRateLimit()
	{
		Name = "Default";
		Limit = 0.f;
	}
	FFrameRateLimit(const FString& Name, const float Limit) : Name(Name), Limit(Limit)
	{
	}
};

USTRUCT()
struct FQualityOption
{
	GENERATED_BODY()
	UPROPERTY()
	FString Name;
	UPROPERTY()
	int32 Value;
	
	FQualityOption()
	{
		Name = "Default";
		Value = 1;
	}
	FQualityOption(const FString& Name, const int32 Value) : Name(Name), Value(Value)
	{
		
	}
};

USTRUCT()
struct FQualitySelector
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;
	
	UPROPERTY()
	UComboBoxString* Selector;

	typedef int (UGameUserSettings::*CallbackGetterFunctionPtr)() const;
	typedef void (UGameUserSettings::*CallbackSetterFunctionPtr)(int32);
	
	CallbackGetterFunctionPtr GetQuality;
	CallbackSetterFunctionPtr SetQuality;

	FQualitySelector()
	{
		Name = "Default";
		Selector = nullptr;
		GetQuality = nullptr;
		SetQuality = nullptr;
	}
	
	FQualitySelector(const FString& Name, UComboBoxString* Selector, CallbackGetterFunctionPtr Getter, CallbackSetterFunctionPtr Setter) :
	Name(Name),
	Selector(Selector),
	GetQuality(Getter),
	SetQuality(Setter)
	{}
};

UCLASS()
class UOptionsMenu : public UMenuWidget
{
	GENERATED_BODY()
	
public:	
	UOptionsMenu(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual bool Initialize() override;
	
	UPROPERTY()
	UGameUserSettings* GameUserSettings;

	// WindowMode
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* WindowModeSelect;
	UPROPERTY()
	TArray<FWindowMode> WindowModeOptions;

	void DefineWindowModeOptions();
	void InitializeWindowModeOptions();
	void GetCurrentWindowMode();
	void SetWindowMode();
	
	// Resolution
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ResolutionSelect;
	UPROPERTY()
	TArray<FResolution> ResolutionOptions;
	
	void DefineResolutionOptions();
	void InitializeResolutionOptions();
	void GetCurrentResolution();
	void SetResolution();

	// FrameRateLimit
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* FrameRateSelect;
	UPROPERTY()
	TArray<FFrameRateLimit> FrameRateLimitOptions;
	
	void DefineFrameRateLimitOptions();
	void InitializeFrameRateLimitOptions();
	void GetCurrentFrameRateLimit();
	void SetFrameRateLimit();

	// QualitySettings
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
	UPROPERTY()
	TArray<FQualitySelector> QualitySelectors;
	UPROPERTY()
	TArray<FQualityOption> QualityOptions;
	
	void DefineQualityOptions();
	void InitializeQualityOptions();
	void GetCurrentQualityOptions();
	void SetQualityOptions();
};
