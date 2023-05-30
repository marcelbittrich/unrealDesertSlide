// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertSlideGameInstance.h"

#include "MenuWidget.h"
#include "RaceManagerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

UDesertSlideGameInstance::UDesertSlideGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWBPClass(TEXT("/Game/DesertSlide/UI/Menu/WBP_MainMenu"));
	MainMenuClass = MainMenuWBPClass.Class;
	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWBPClass(TEXT("/Game/DesertSlide/UI/Menu/WBP_InGameMenu"));
	InGameMenuClass = InGameMenuWBPClass.Class;
}

void UDesertSlideGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("GameInstace Initialized"));
	
}

void UDesertSlideGameInstance::LoadMainMenu()
{
	if (!MainMenuClass) return;
	MainMenu = CreateWidget<UMenuWidget>(this, MainMenuClass);
	if (!MainMenu) return;
	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UDesertSlideGameInstance::LoadInGameMenu()
{
	if (!InGameMenuClass) return;
	InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!InGameMenu) return;
	InGameMenu->Setup();
	InGameMenu->SetMenuInterface(this);
}

void UDesertSlideGameInstance::LoadSoloLevel()
{
	if (MainMenu)
	{
		MainMenu->Teardown();
	}
	
	UWorld* World = GetWorld();
	if(!World) return;

	World->ServerTravel("/Game/DesertSlide/Maps/MovementTestMap");
}

void UDesertSlideGameInstance::GoToMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	
	if (PlayerController)
	{
		PlayerController->ClientTravel("/Game/PuzzlePlatforms/Maps/MainMenu", TRAVEL_Absolute);
	}
}

void UDesertSlideGameInstance::QuitGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Quit Game Instance"));
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	
	if (PlayerController)
	{
		PlayerController->ConsoleCommand("Quit");

	}
}

void UDesertSlideGameInstance::RestartLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to restart"));
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), true);
}

void UDesertSlideGameInstance::StartRace()
{
	URaceManagerSubsystem* RaceManager = GetSubsystem<URaceManagerSubsystem>(this);
	if (RaceManager)
	{
		RaceManager->InitializeRace();
	}
}


