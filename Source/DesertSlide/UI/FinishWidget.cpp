// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishWidget.h"

#include "Components/TextBlock.h"

void UFinishWidget::Display(float LapTime, int FinishedLap, bool bRaceFinished, float DisplayTime)
{
	HeadlineText->SetText(GetHeadlineText(FinishedLap, bRaceFinished));
	FString TimeTextString = "Time: " + FString::SanitizeFloat(LapTime);
	TimeText->SetText(FText::FromString(TimeTextString));

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	
	if (PlayerController)
	{
		Setup();
		
		FTimerHandle DisplayCheckpointTimerHandle;
		FTimerDelegate DisplayCheckpointTimerDelegate = FTimerDelegate::CreateUObject(
			this,
			&UFinishWidget::Teardown
		);
	
		PlayerController->GetWorldTimerManager().SetTimer(
			DisplayCheckpointTimerHandle,
			DisplayCheckpointTimerDelegate,
			DisplayTime,
			false
		);
	}

}

FText UFinishWidget::GetHeadlineText(int Lap, bool bRaceFinished)
{
	FString HeadlineTextString;
	if (bRaceFinished)
	{
		HeadlineTextString = "Finish!";
	}
	else
	{
		if (Lap == 1)
		{
			HeadlineTextString = "1st Lap";
		}
		else if (Lap == 2)
		{
			HeadlineTextString = "2nd Lap";
		}
		else if (Lap > 2)
		{
			HeadlineTextString = FString::FromInt(Lap) + "th Lap";
		}
		else
		{
			HeadlineTextString = "Round";
		}
	}
	return FText::FromString(HeadlineTextString);
}
