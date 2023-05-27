// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertSlideGameInstance.h"

#include "MenuWidget.h"
#include "RaceManagerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

UDesertSlideGameInstance::UDesertSlideGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWBPClass(TEXT("/Game/DesertSlide/UI/Menu/WBP_InGameMenu"));
	InGameMenuClass = InGameMenuWBPClass.Class;
}

void UDesertSlideGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("GameInstace Initialized"));
	
}

void UDesertSlideGameInstance::LoadInGameMenu()
{
	if (!InGameMenuClass) return;
	InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!InGameMenu) return;
	InGameMenu->Setup();
	InGameMenu->SetMenuInterface(this);
}

void UDesertSlideGameInstance::QuitGame()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	
	if (PlayerController)
	{
		PlayerController->ConsoleCommand("Quit");
	}
}

void UDesertSlideGameInstance::RestartLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to restart"));
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void UDesertSlideGameInstance::StartRace()
{
	URaceManagerSubsystem* RaceManager = GetSubsystem<URaceManagerSubsystem>(this);
	if (RaceManager)
	{
		RaceManager->InitializeRace();
	}
}
