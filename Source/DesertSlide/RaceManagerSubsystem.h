// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RaceManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API URaceManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void StartCrossed(AActor* TriggeringActor);
	void FinishCrossed(AActor* TriggeringActor);
	void CheckpointCrossed(AActor* Checkpoint, AActor* TriggeringActor);

	void SetLaps(uint8 Laps);
	
private:
	void ClearData();
	bool IsNewCheckpoint(const AActor* Checkpoint);
	
	UPROPERTY()
	TArray<AActor*> AllCheckpoints;
	UPROPERTY()
	TArray<AActor*> CrossedCheckpoints;
	
	bool bStartCrossed = false;
	bool bAllCheckpointsCrossed = false;

	uint8 Laps;
	uint8 CurrentLap;
};

