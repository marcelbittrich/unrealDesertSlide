// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertSlideGameInstance.h"

#include "DesertSlideSaveGame.h"
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
		LoadMapSaveGame();
		RaceManager->InitializeRace();
	}
}

void UDesertSlideGameInstance::LoadMapSaveGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Tried Loading SaveGame for %s"), *GetWorld()->GetMapName());
	LoadSaveGame(GetWorld()->GetMapName());
}

void UDesertSlideGameInstance::WriteMapSaveGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Tried Saving SaveGame for %s"), *GetWorld()->GetMapName());
	WriteSaveGame(GetWorld()->GetMapName());
}

void UDesertSlideGameInstance::LoadSaveGame(const FString& MapName)
{
	if (!UGameplayStatics::DoesSaveGameExist(MapName, UserIndex))
	{
		SaveGame = Cast<UDesertSlideSaveGame>(UGameplayStatics::CreateSaveGameObject(UDesertSlideSaveGame::StaticClass()));
		if (SaveGame)
		{
			UE_LOG(LogTemp, Warning, TEXT("Created new SaveGame"));
		}
	}
	else
	{
		if (Cast<UDesertSlideSaveGame>(UGameplayStatics::LoadGameFromSlot(MapName, UserIndex)))
		{
			SaveGame = Cast<UDesertSlideSaveGame>(UGameplayStatics::LoadGameFromSlot(MapName, UserIndex));
			UE_LOG(LogTemp, Warning, TEXT("Loaded existing SaveGame"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("LoadSaveGame failed"));
		}
	}
}

void UDesertSlideGameInstance::WriteSaveGame(const FString& MapName)
{
	if (SaveGame)
	{
		UGameplayStatics::AsyncSaveGameToSlot(SaveGame, MapName, UserIndex);
	}
}


