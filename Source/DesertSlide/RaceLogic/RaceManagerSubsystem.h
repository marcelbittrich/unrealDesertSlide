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

	URaceManagerSubsystem();
	
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void InitializeRace();

	void StartCrossed(AActor* TriggeringActor);
	void FinishCrossed(AActor* TriggeringActor);
	void CheckpointCrossed(AActor* Checkpoint, AActor* TriggeringActor);

	void HandleRaceStart();
	void HandleNewLap();
	void HandleRaceEnd();
	void HandleNewPersonalBest();

	float GetCurrentLapTime();

	UFUNCTION(BlueprintCallable)
	void SetLaps(uint8 Laps);

	UFUNCTION(BlueprintCallable)
	void AddTimingsUI();

	UFUNCTION(BlueprintCallable)
	void UpdateLapTimeUI();

	UFUNCTION()
	void UpdateLastLapTimeUI();

	UFUNCTION()
	void UpdateCurrentLapUI();

	UFUNCTION()
	void UpdatePersonalBestTimeUI();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetStartCountdownTime() const {return StartCountdownTime;};
	
private:
	void ClearData();
	bool IsNewCheckpoint(const AActor* Checkpoint);
	
	UPROPERTY()
	TArray<AActor*> AllCheckpoints;
	UPROPERTY()
	TArray<AActor*> CrossedCheckpoints;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bRaceInitialized = false;
	bool bStartCrossed = false;
	bool bAllCheckpointsCrossed = false;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bRaceEnded = false;

	float StartCountdownTime = 3;
	
	uint8 Laps = 2;
	uint8 CurrentLap = 1;
	float RaceStartTime = 0;
	float RaceEndTime = 0;
	float LapStartTime = 0;
	float LastLapTime = 0;

	float PersonalBestTime = 0;
	
	UPROPERTY()
	TSubclassOf<class ACheckpoint> CheckpointClass;
	UPROPERTY()
	TSubclassOf<class UUserWidget> TimingsUIClass;
	UPROPERTY()
	class UTimings* TimingsUI;
	UPROPERTY()
	TSubclassOf<class UUserWidget> CheckpointUIClass;
	UPROPERTY()
	class UCheckpointWidget* CheckpointUI;
	UPROPERTY()
	TSubclassOf<class UUserWidget> FinishUIClass;
	UPROPERTY()
	class UFinishWidget* FinishUI;
	UPROPERTY()
	TSubclassOf<class UUserWidget> StartUIClass;
	UPROPERTY()
	class UStartWidget* StartUI;
};

