// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointWidget.h"

#include "Components/TextBlock.h"

void UCheckpointWidget::Setup()
{
	this->AddToViewport();
}

void UCheckpointWidget::Teardown()
{
	this->RemoveFromParent();
}

void UCheckpointWidget::Display(float SectorTime, float DisplayTime)
{
	FString SectorTimeString = "Time: " + FString::SanitizeFloat(SectorTime);
	SectorTimeText->SetText(FText::FromString(SectorTimeString));

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	
	if (PlayerController)
	{
		Setup();
		
		FTimerHandle DisplayCheckpointTimerHandle;
		FTimerDelegate DisplayCheckpointTimerDelegate = FTimerDelegate::CreateUObject(
			this,
			&UCheckpointWidget::Teardown
		);
	
		PlayerController->GetWorldTimerManager().SetTimer(
			DisplayCheckpointTimerHandle,
			DisplayCheckpointTimerDelegate,
			DisplayTime,
			false
		);
	}
}

