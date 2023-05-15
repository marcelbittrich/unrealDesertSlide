// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceManagerSubsystem.h"

#include "Checkpoint.h"
#include "Kismet/GameplayStatics.h"

void URaceManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FlushData();
	UE_LOG(LogTemp, Warning, TEXT("Race Manager Initialized"));
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpoint::StaticClass(), AllCheckpoints);
	
	UE_LOG(LogTemp, Warning, TEXT("Found %i Checkpoints"), AllCheckpoints.Num());
}

void URaceManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	FlushData();
	UE_LOG(LogTemp, Warning, TEXT("Race Manager Deinitialized"));
}

void URaceManagerSubsystem::CheckpointCrossed(AActor* Checkpoint, AActor* TriggeringActor)
{
	UE_LOG(LogTemp, Warning, TEXT("RaceManager: Checkpoint %s triggerd by: %s"), *Checkpoint->GetActorNameOrLabel(), *TriggeringActor->GetActorNameOrLabel());

	if (IsNewCheckpoint(Checkpoint))
	{
		CrossedCheckpoints.Add(Checkpoint);
		if (CrossedCheckpoints.Num() == AllCheckpoints.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("All Checkpoints Crossed!"));
		}
	}
}

void URaceManagerSubsystem::FlushData()
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
