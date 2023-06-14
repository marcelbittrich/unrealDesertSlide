// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if(!StartRaceInGameMenuButton) return false;
	StartRaceInGameMenuButton->OnClicked.AddDynamic(this, &UInGameMenu::StartRace);

	if(!RestartInGameMenuButton) return false;
	RestartInGameMenuButton->OnClicked.AddDynamic(this, &UInGameMenu::Restart);
	
	if(!CancelInGameMenuButton) return false;
	CancelInGameMenuButton->OnClicked.AddDynamic(this, &UInGameMenu::Cancel);

	if (!LeaveGameInGameMenuButton) return false;
	LeaveGameInGameMenuButton->OnClicked.AddDynamic(this, &UInGameMenu::LeaveGame);

	//if (!ReadyToRaceCheckbox) return false;
	//ReadyToRaceCheckbox->OnCheckStateChanged.AddDynamic(this, &UInGameMenu::HandleReadyToRaceChange);
	
	if (!FollowCamCheckBox) return false;
	FollowCamCheckBox->OnCheckStateChanged.AddDynamic(this, &UInGameMenu::HandleFollowCamChange);
	
	return true;
}

void UInGameMenu::StartRace()
{
	if(MenuInterface)
	{
		Teardown();
		MenuInterface->StartRace();
	}
}

void UInGameMenu::HandleReadyToRaceChange(bool bReady)
{
	if(MenuInterface)
	{
		//MenuInterface->HandlePlayerReadyChange(bReady);
	}
}

void UInGameMenu::HandleFollowCamChange(bool bFollowCam)
{
	if (MenuInterface)
	{
		MenuInterface->HandleFollowCamChange(bFollowCam);
	}
}

void UInGameMenu::Restart()
{
	if(MenuInterface)
	{
		Teardown();
		MenuInterface->RestartLevel();
	}
}

void UInGameMenu::Cancel()
{
	Teardown();	
}

void UInGameMenu::LeaveGame()
{
	if(MenuInterface)
	{
		Teardown();
		MenuInterface->GoToMainMenu();
	}
}
