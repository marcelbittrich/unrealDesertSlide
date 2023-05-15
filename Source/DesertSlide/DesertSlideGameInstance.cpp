// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertSlideGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "UI/Timings.h"
#include "UObject/ConstructorHelpers.h"

UDesertSlideGameInstance::UDesertSlideGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> TimingsUIWBPClass(TEXT("/Game/DesertSlide/UI/WBP_Timings"));
	TimingsUIClass = TimingsUIWBPClass.Class;
}

void UDesertSlideGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("GameInstace Initialized"));
	RaceTime = 0;

	
}

void UDesertSlideGameInstance::CheckPointCrossed()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstace Checkpoint crossed!"));
}

void UDesertSlideGameInstance::AddTimingsUI()
{
	if(!TimingsUIClass) return;
	TimingsUI = CreateWidget<UTimings>(this, TimingsUIClass);

	if (TimingsUI)
	{
		TimingsUI->Setup();
	}
}

void UDesertSlideGameInstance::UpdateTimings()
{
	float WorldTime = GetWorld()->GetTime().GetRealTimeSeconds();
	if (TimingsUI)
	{
		TimingsUI->SetGameTime(WorldTime);
	}
}
