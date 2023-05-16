// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceManagerSubsystem.h"

#include "Checkpoint.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Timings.h"

URaceManagerSubsystem::URaceManagerSubsystem()
{
	ConstructorHelpers::FClassFinder<UUserWidget> TimingsUIWBPClass(TEXT("/Game/DesertSlide/UI/WBP_Timings"));
	TimingsUIClass = TimingsUIWBPClass.Class;
}

void URaceManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ClearData();
	UE_LOG(LogTemp, Warning, TEXT("Race Manager Initialized"));
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpoint::StaticClass(), AllCheckpoints);
	
	UE_LOG(LogTemp, Warning, TEXT("Found %i Checkpoints"), AllCheckpoints.Num());
}

void URaceManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	ClearData();
	UE_LOG(LogTemp, Warning, TEXT("Race Manager Deinitialized"));
}

void URaceManagerSubsystem::StartCrossed(AActor* TriggeringActor)
{
	if (bStartCrossed == false)
	{
		RaceStartTime = GetWorld()->GetTime().GetWorldTimeSeconds();
		LapStartTime = GetWorld()->GetTime().GetWorldTimeSeconds();
		bStartCrossed = true;
		UE_LOG(LogTemp, Warning, TEXT("Race Start"));
	}
}

void URaceManagerSubsystem::FinishCrossed(AActor* TriggeringActor)
{
	if (bAllCheckpointsCrossed)
	{
		if (CurrentLap < Laps)
		{
			HandleNewLap();
			UE_LOG(LogTemp, Warning, TEXT("New Lap"));
		}
		else
		{
			HandleRaceEnd();
			UE_LOG(LogTemp, Warning, TEXT("Race Over"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not all Checkpoints crossed"));
	}
}

void URaceManagerSubsystem::HandleNewLap()
{
	LastLapTime = GetCurrentLapTime();

	LapStartTime = GetWorld()->GetTime().GetWorldTimeSeconds();

	++CurrentLap;
	
	bAllCheckpointsCrossed = false;
	CrossedCheckpoints.Empty();
	
	UpdateLastLapTimeUI();
	UpdateCurrentLapUI();
}

void URaceManagerSubsystem::HandleRaceEnd()
{
	RaceEndTime = GetCurrentLapTime();
	
	UpdateLastLapTimeUI();
	UpdateCurrentLapUI();
	bRaceEnded = true;
}

void URaceManagerSubsystem::CheckpointCrossed(AActor* Checkpoint, AActor* TriggeringActor)
{
	if (!bStartCrossed) return;
		
	UE_LOG(LogTemp, Warning, TEXT("RaceManager: Checkpoint %s triggerd by: %s"), *Checkpoint->GetActorNameOrLabel(), *TriggeringActor->GetActorNameOrLabel());

	if (IsNewCheckpoint(Checkpoint))
	{
		CrossedCheckpoints.Add(Checkpoint);
		if (CrossedCheckpoints.Num() == AllCheckpoints.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("All Checkpoints Crossed!"));
			bAllCheckpointsCrossed = true;
		}
	}
}

float URaceManagerSubsystem::GetCurrentLapTime()
{
	if (bRaceEnded)
	{
		return RaceEndTime;
	}
	
	if (RaceStartTime != 0.0f)
	{
		float WorldTime = GetWorld()->GetTime().GetWorldTimeSeconds();
		return WorldTime - LapStartTime;
	}
	
	return 0.0f;
}

void URaceManagerSubsystem::SetLaps(uint8 LapNumber)
{
	Laps = LapNumber;
}

void URaceManagerSubsystem::ClearData()
{
	CrossedCheckpoints.Empty();
}

bool URaceManagerSubsystem::IsNewCheckpoint(const AActor* NewCheckpoint)
{
	if (!ensure(NewCheckpoint)) return false;

	for (const AActor* Checkpoint : CrossedCheckpoints)
	{
		if (Checkpoint->GetActorNameOrLabel() == NewCheckpoint->GetActorNameOrLabel())
		{
			return false;
		}
	}
	return true;
}

void URaceManagerSubsystem::AddTimingsUI()
{
	if(!TimingsUIClass) return;
	TimingsUI = CreateWidget<UTimings>(this->GetGameInstance(), TimingsUIClass);

	if (TimingsUI)
	{
		TimingsUI->Setup();
	}

	UpdateLapTimeUI();
	UpdateCurrentLapUI();
	UpdateLastLapTimeUI();
}

void URaceManagerSubsystem::UpdateLapTimeUI()
{
	if (TimingsUI)
	{
		TimingsUI->SetLapTimeText(GetCurrentLapTime());
	}
}

void URaceManagerSubsystem::UpdateLastLapTimeUI()
{
	if (TimingsUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("RaceManager: SetLastLapTime %f"), LastLapTime);
		TimingsUI->SetLastLapTimeText(LastLapTime);
	}
}

void URaceManagerSubsystem::UpdateCurrentLapUI()
{
	if (TimingsUI)
	{
		TimingsUI->SetCurrentLapText(CurrentLap, Laps);
	}
}

