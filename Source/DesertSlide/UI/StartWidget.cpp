// Fill out your copyright notice in the Description page of Project Settings.


#include "StartWidget.h"

void UStartWidget::Display(float DisplayTime)
{
	Setup();
	
	/*
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	
	if (PlayerController)
	{
		Setup();
		
		FTimerHandle DisplayCheckpointTimerHandle;
		FTimerDelegate DisplayCheckpointTimerDelegate = FTimerDelegate::CreateUObject(
			this,
			&UStartWidget::Teardown
		);
	
		PlayerController->GetWorldTimerManager().SetTimer(
			DisplayCheckpointTimerHandle,
			DisplayCheckpointTimerDelegate,
			DisplayTime,
			false
		);
	}
	*/
}
