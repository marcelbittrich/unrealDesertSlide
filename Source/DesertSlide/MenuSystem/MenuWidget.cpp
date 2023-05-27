// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* Interface)
{
	this->MenuInterface = Interface;
}

void UMenuWidget::Setup()
{
	this->AddToViewport();
	this->bIsFocusable = true;

	UWorld* World = GetWorld();
	if (!World) return;
	
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController)
	{
		FInputModeUIOnly UIInputMode;
		UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		UIInputMode.SetWidgetToFocus(this->TakeWidget());
		
		PlayerController->SetInputMode(UIInputMode);
		PlayerController->SetShowMouseCursor(true);
	}
}

void UMenuWidget::Teardown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!World) return;
	
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController)
	{
		FInputModeGameOnly GameInputMode;
		PlayerController->SetInputMode(GameInputMode);
		PlayerController->SetShowMouseCursor(false);
	}
}
