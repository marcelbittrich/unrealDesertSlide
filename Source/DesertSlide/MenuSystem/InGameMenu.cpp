// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"

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
	LeaveGameInGameMenuButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitGame);
	
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

void UInGameMenu::QuitGame()
{
	if(MenuInterface)
	{
		Teardown();
		MenuInterface->QuitGame();
	}
}
