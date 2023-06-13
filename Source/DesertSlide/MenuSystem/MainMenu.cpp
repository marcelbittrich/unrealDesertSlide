// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "ServerRow.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UObject/Class.h"
#include "GameFramework/GameUserSettings.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UServerRow> ServerRowWBPClass(TEXT("/Game/DesertSlide/UI/Menu/WBP_ServerRow"));
	ServerRowClass = ServerRowWBPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;
	
	Success = InitSelector();
	if (!Success) return false;
	Success = InitOptions();
	if (!Success) return false;
	Success = InitWarning();
	if (!Success) return false;
	Success = InitMultiplayerMenu();
	if (!Success) return false;
	Success = InitHostMenu();
	if (!Success) return false;
	Success = InitJoinMenu();
	if (!Success) return false;
	
	return true;
}

#pragma region MenuCore

bool UMainMenu::InitSelector()
{
	if (!SoloButton) return false;
	SoloButton->OnClicked.AddDynamic(this, &UMainMenu::StartSoloGame);

	if (!MultiplayerButton) return false;
	MultiplayerButton->OnClicked.AddDynamic(this, &UMainMenu::OpenWarning);

	if (!OptionsButton) return false;
	OptionsButton->OnClicked.AddDynamic(this, &UMainMenu::OpenOptionsMenu);
	
	if (!QuitGameButton) return false;
	QuitGameButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	
	if (!ExitButton) return false;
	ExitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	return true;
}

bool UMainMenu::InitOptions()
{
	if (!ApplySettingsButton) return false;
	ApplySettingsButton->OnClicked.AddDynamic(this, &UMainMenu::ApplySettings);

	return true;
}

bool UMainMenu::InitWarning()
{
	if (!BackWarningButton) return false;
	BackWarningButton->OnClicked.AddDynamic(this, &UMainMenu::OpenSelector);
	if (!GotItWarningButton) return false;
	GotItWarningButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMultiplayerMenu);
	
	return true;
}

bool UMainMenu::InitMultiplayerMenu()
{
	if (!JoinMultiplayerButton) return false;
	JoinMultiplayerButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	if (!HostMultiplayerButton) return false;
	HostMultiplayerButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);
	if (!BackMultiplayerButton) return false;
	BackMultiplayerButton->OnClicked.AddDynamic(this, &UMainMenu::OpenSelector);
	
	return true;
}

bool UMainMenu::InitHostMenu()
{
	if (!CancelHostMenuButton) return false;
	CancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMultiplayerMenu);
	if (!HostHostMenuButton) return false;
	HostHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	
	return true;
}

bool UMainMenu::InitJoinMenu()
{
	if (!CancelJoinMenuButton) return false;
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMultiplayerMenu);
	if (!ConnectJoinMenuButton) return false;
	ConnectJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	
	return true;
}

void UMainMenu::OpenSelector()
{
	if (MenuSwitcher && Selector)
	{
		MenuSwitcher->SetActiveWidget(Selector);
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

void UMainMenu::OpenWarning()
{
	if (MenuSwitcher && Warning)
	{
		MenuSwitcher->SetActiveWidget(Warning);
	}
}

void UMainMenu::OpenMultiplayerMenu()
{
	if (MenuSwitcher && MultiplayerMenu)
	{
		MenuSwitcher->SetActiveWidget(MultiplayerMenu);
	}
}

void UMainMenu::OpenHostMenu()
{
	if (MenuSwitcher && HostMenu)
	{
		MenuSwitcher->SetActiveWidget(HostMenu);
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (MenuInterface)
	{
		MenuInterface->RefreshServerList();
	}
	
	if (MenuSwitcher && JoinMenu)
	{
		MenuSwitcher->SetActiveWidget(JoinMenu);
	}

	if (SearchingText)
	{
		SearchingText->SetVisibility(ESlateVisibility::Visible);
	}
}

# pragma endregion 

void UMainMenu::StartSoloGame()
{
	if (MenuInterface)
	{
		MenuInterface->LoadSoloLevel();
	}
}

void UMainMenu::SetServerList(TArray<FServerData>& Servers)
{
	if (SearchingText)
	{
		SearchingText->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (ServerList)
	{
		ServerList->ClearChildren();

		uint32 ServerIndex = 0;
		for (FServerData& Server : Servers)
		{
			ServerRow = CreateWidget<UServerRow>(this, ServerRowClass);
		
			ServerRow->ServerName->SetText(FText::FromString(Server.Name));
			ServerRow->HostUser->SetText(FText::FromString(Server.HostUserName));

			FString ConnectionFraction = FString::Printf(TEXT("%d/%d"), Server.CurrentPlayers, Server.MaxPlayers);
			ServerRow->ConnectionFraction->SetText(FText::FromString(ConnectionFraction));

			ServerRow->Setup(this, ServerIndex);
			++ServerIndex;
			ServerList->AddChild(ServerRow);
		}
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateServerRows();	
}

void UMainMenu::UpdateServerRows()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		UServerRow* Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (!Row) break;
		
		Row->bSelected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
	}
}

void UMainMenu::HostServer()
{
	FString ServerName;
	if (ServerNameTextBox && ServerNameTextBox->GetText().ToString() != "")
	{
		ServerName = ServerNameTextBox->GetText().ToString();
	}
	else
	{
		ServerName = "Default";
	}
	
	UE_LOG(LogTemp, Warning, TEXT("I want to host a Server!"));
	if (MenuInterface)
	{
		MenuInterface->Host(ServerName);
	}
}

void UMainMenu::JoinServer()
{
	if(MenuInterface && SelectedIndex.IsSet())
	{
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SlectedIndex not set"));
	}
}

void UMainMenu::ReadCurrentSettings()
{
	if (!GameUserSettings) return;

	UE_LOG(LogTemp, Warning, TEXT("Config Dir: %s"), *GGameUserSettingsIni);
	
	GetCurrentWindowMode();
	GetCurrentResolution();
	GetCurrentFrameRateLimit();
	GetCurrentQualityOptions();
}

void UMainMenu::ApplySettings()
{
	OpenSelector();

	if (!GameUserSettings)
	{
		return;
	}
	
	SetWindowMode();
	SetResolution();
	SetFrameRateLimit();
	SetQualityOptions();
	
	GameUserSettings->ApplySettings(false);
}

void UMainMenu::QuitGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Quit Game"));
	if (MenuInterface)
	{
		MenuInterface->QuitGame();
	}
}
