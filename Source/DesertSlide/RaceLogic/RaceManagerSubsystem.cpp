// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceManagerSubsystem.h"

#include "Checkpoint.h"
#include "DesertSlidePlayerController.h"
#include "FinishWidget.h"
#include "StartWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CheckpointWidget.h"
#include "UI/Timings.h"

URaceManagerSubsystem::URaceManagerSubsystem()
{
	ConstructorHelpers::FClassFinder<ACheckpoint> CheckpointBPClass(TEXT("/Game/DesertSlide/Core/Actors/BP_Checkpoint"));
	CheckpointClass = CheckpointBPClass.Class;
	ConstructorHelpers::FClassFinder<UUserWidget> TimingsUIWBPClass(TEXT("/Game/DesertSlide/UI/WBP_Timings"));
	TimingsUIClass = TimingsUIWBPClass.Class;
	ConstructorHelpers::FClassFinder<UUserWidget> CheckpointUIWBPClass(TEXT("/Game/DesertSlide/UI/WBP_CheckpointWidget"));
	CheckpointUIClass = CheckpointUIWBPClass.Class;
	ConstructorHelpers::FClassFinder<UUserWidget> FinishUIWBPClass(TEXT("/Game/DesertSlide/UI/WBP_FinishWidget"));
	FinishUIClass = FinishUIWBPClass.Class;
	ConstructorHelpers::FClassFinder<UUserWidget> StartUIWBPClass(TEXT("/Game/DesertSlide/UI/WBP_StartWidget"));
	StartUIClass = StartUIWBPClass.Class;
}

void URaceManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if(!TimingsUIClass) return;
	TimingsUI = CreateWidget<UTimings>(this->GetGameInstance(), TimingsUIClass);
	if(!CheckpointUIClass) return;
	CheckpointUI = CreateWidget<UCheckpointWidget>(this->GetGameInstance(), CheckpointUIClass);
	if(!FinishUIClass) return;
	FinishUI = CreateWidget<UFinishWidget>(this->GetGameInstance(), FinishUIClass);
	if(!StartUIClass) return;
	StartUI = CreateWidget<UStartWidget>(this->GetGameInstance(), StartUIClass);
	
	UE_LOG(LogTemp, Warning, TEXT("Race Manager Initialized"));
}

void URaceManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	ClearData();
	UE_LOG(LogTemp, Warning, TEXT("Race Manager Deinitialized"));
}

void URaceManagerSubsystem::InitializeRace()
{
	ClearData();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CheckpointClass, AllCheckpoints);
	UE_LOG(LogTemp, Warning, TEXT("Found %i Checkpoints"), AllCheckpoints.Num());
	
	AddTimingsUI();
	bRaceInitialized = true;
	UE_LOG(LogTemp, Warning, TEXT("Race Manager RaceInitialized"));
	HandleRaceStart();
}

void URaceManagerSubsystem::HandleRaceStart()
{
	UE_LOG(LogTemp, Warning, TEXT("HandleRaceStart"));
	ADesertSlidePlayerController*  DesertSlidePlayerController = Cast<ADesertSlidePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	if (DesertSlidePlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleRaceStart Disable Input"));
		DesertSlidePlayerController->SetPlayerEnabledState(false);
		StartUI->Display();
		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
			DesertSlidePlayerController,
			&ADesertSlidePlayerController::SetPlayerEnabledState,
			true
		);
		DesertSlidePlayerController->GetWorldTimerManager().SetTimer(
			PlayerEnableTimerHandle,
			PlayerEnableTimerDelegate,
			StartCountdownTime,
			false
		);
	}
}

void URaceManagerSubsystem::StartCrossed(AActor* TriggeringActor)
{
	if (!bRaceInitialized || bRaceEnded) return;
	
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
	if (!bRaceInitialized || bRaceEnded) return;
	
	if (bAllCheckpointsCrossed)
	{
		bool bDisplayFinished = ( CurrentLap == Laps );
		FinishUI->Display(GetCurrentLapTime(), CurrentLap, bDisplayFinished);
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
	if (!bRaceInitialized || !bStartCrossed || bRaceEnded) return;
		
	UE_LOG(LogTemp, Warning, TEXT("RaceManager: Checkpoint %s triggerd by: %s"), *Checkpoint->GetActorNameOrLabel(), *TriggeringActor->GetActorNameOrLabel());

	if (IsNewCheckpoint(Checkpoint))
	{
		if (CheckpointUI)
		{
			CheckpointUI->Display(GetCurrentLapTime());
		}
		
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
	// TODO: Extend to achieve clean start after every initialize
	CrossedCheckpoints.Empty();
	CurrentLap = 1;
	RaceStartTime = 0;
	RaceEndTime = 0;
	LapStartTime = 0;
	LastLapTime = 0;
	bRaceInitialized = false;
	bStartCrossed = false;
	bAllCheckpointsCrossed = false;
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

