// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertSlideGameInstance.h"

#include "DesertSlidePlayerController.h"
#include "DesertSlideSaveGame.h"
#include "MainMenu.h"
#include "MenuWidget.h"
#include "RaceManagerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"

const static FName SESSION_NAME = NAME_GameSession;
const static FName SERVERNAME_SETTINGS_KEY = TEXT("ServerName");

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

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	
	if (OnlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found OnlineSubsystem: %s"), *OnlineSubsystem->GetSubsystemName().ToString());
		
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UDesertSlideGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UDesertSlideGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UDesertSlideGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UDesertSlideGameInstance::OnJoinSessionComplete);
		}
	}

	if (GEngine)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UDesertSlideGameInstance::OnNetworkFailure);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("GameInstace Initialized"));
	
}

void UDesertSlideGameInstance::LoadMainMenu()
{
	if (!MainMenuClass) return;
	MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);
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

	World->ServerTravel("/Game/DesertSlide/Maps/RacePrototype02");
}

void UDesertSlideGameInstance::GoToMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	
	if (PlayerController)
	{
		PlayerController->ClientTravel("/Game/DesertSlide/Maps/MainMenuMap", TRAVEL_Absolute);
	}
	
	URaceManagerSubsystem* RaceManager = GetSubsystem<URaceManagerSubsystem>(this);
	if (RaceManager)
	{
		RaceManager->DeinitializeRace();
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

void UDesertSlideGameInstance::HandleFollowCamChange(bool bFollowCamChange)
{
	ADesertSlidePlayerController* PlayerController = Cast<ADesertSlidePlayerController>(GetFirstLocalPlayerController());
	if (!PlayerController) return;

	PlayerController->bFollowCam = bFollowCamChange;
}

#pragma region SaveGame

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

#pragma endregion 

void UDesertSlideGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		UE_LOG(LogTemp, Warning, TEXT("Start Session Search"),);
	}
}

void UDesertSlideGameInstance::Host(FString& ServerName)
{
	NewServerName = ServerName;
	
	if (SessionInterface.IsValid())
	{
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();	
		}
	}
}

void UDesertSlideGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UDesertSlideGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UDesertSlideGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		
		SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
		
		SessionSettings.NumPublicConnections = 4;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.Set(SERVERNAME_SETTINGS_KEY, NewServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UDesertSlideGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if(!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create Session"));
		return;
	}
	
	if (MainMenu)
	{
		MainMenu->Teardown();
	}
	
	UEngine* Engine = GetEngine();
	if (!Engine) return;

	Engine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if(!World) return;

	World->ServerTravel("/Game/DesertSlide/Maps/RacePrototype02?listen");
}

void UDesertSlideGameInstance::OnFindSessionsComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && MainMenu)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Search Finished"));
		
		TArray<FOnlineSessionSearchResult> FoundSessions = SessionSearch->SearchResults;
		TArray<FServerData> ServerNames;
		
		for (FOnlineSessionSearchResult& SearchResult : FoundSessions)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Session: %s"), *SearchResult.GetSessionIdStr());
			FServerData Data;
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUserName = SearchResult.Session.OwningUserName;
			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVERNAME_SETTINGS_KEY, ServerName))
				Data.Name = ServerName;
			
			ServerNames.Add(Data);
		}
		
		MainMenu->SetServerList(ServerNames);
	}
}

void UDesertSlideGameInstance::Join(uint32 Index)
{
	if (!SessionInterface) return;
	if (!SessionSearch) return;
	
	if (MainMenu)
	{
		MainMenu->Teardown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UDesertSlideGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	FString ConnectInfo;
	bool Success = SessionInterface->GetResolvedConnectString(SessionName, ConnectInfo);

	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
		return;
	}
	
	if (UEngine* Engine = GetEngine())
	{
		const FString JoinString = "Joining " + ConnectInfo;
		Engine->AddOnScreenDebugMessage(-1, 5, FColor::Green, *JoinString);
	}
	
	if (APlayerController* PlayerController = GetFirstLocalPlayerController())
	{
		PlayerController->ClientTravel(ConnectInfo, TRAVEL_Absolute);
	}
}

void UDesertSlideGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType,
	const FString& ErrorString)
{
	GoToMainMenu();
}