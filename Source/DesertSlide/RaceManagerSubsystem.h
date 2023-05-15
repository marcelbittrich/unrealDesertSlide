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

	void CheckpointCrossed(AActor* Checkpoint, AActor* TriggeringActor);

private:
	void FlushData();

	UPROPERTY()
	TArray<AActor*> AllCheckpoints;
	
	UPROPERTY()
	TArray<AActor*> CrossedCheckpoints;
	
	bool IsNewCheckpoint(const AActor* Checkpoint);
};
