// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertSlidePlayerController.h"

#include "DesertSlideGameState.h"
#include "DesertSlidePlayerState.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void ADesertSlidePlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
	if (bPlayerEnabled)
	{
		GetPawn()->EnableInput(this);
	}
	else
	{
		GetPawn()->DisableInput(this);
	}
}

void ADesertSlidePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bFollowCam && bAutoRotate)
	{
		AutoRotateCamera(DeltaSeconds);
	}
	
	int i = 0;
	for (FPlayerData& PlayerData : LocalPlayerReadinessStates)
	{
		FString ReadyMessage = PlayerData.ReadyToRace ? FString("ready") : FString("not ready");
		FString DebugMessage = PlayerData.Name + " is " + ReadyMessage;
		GEngine->AddOnScreenDebugMessage(++i,-1, FColor::Green, *DebugMessage);
	}
}

void ADesertSlidePlayerController::AutoRotateCamera(float DeltaSeconds)
{
	ACharacter* DesertCharacter = GetCharacter();

	if (!DesertCharacter) return;
	
	FVector Velocity = DesertCharacter->GetVelocity();
	
	if (Velocity.SquaredLength() >= FMath::Square(AutoRotateMinVelocity))
	{
		FRotator CurrentControlRotation = GetControlRotation();
		FVector CameraViewDirection = CurrentControlRotation.Vector();
		FVector VelocityDirection = Velocity.GetSafeNormal();

		FRotator DeltaRotation = FRotator(-AutoRotatePitchAngle, VelocityDirection.Rotation().Yaw, 0) - CameraViewDirection.Rotation();
		FRotator PartialDeltaRotation = FMath::InterpEaseInOut(FRotator::ZeroRotator, DeltaRotation, AutoRotateInterpSpeed * DeltaSeconds, 1.f);

		SetControlRotation(CurrentControlRotation + PartialDeltaRotation);
	}
}

void ADesertSlidePlayerController::FlipReadyToRace()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		Server_SetReadyToRace();
		UE_LOG(LogTemp, Warning, TEXT("Call SetReadyToRace"));
	}
	else
	{
		ADesertSlidePlayerState* DesertSlidePlayerState = GetPlayerState<ADesertSlidePlayerState>();
		if (!DesertSlidePlayerState) return;
	
		ADesertSlideGameState* GameState = Cast<ADesertSlideGameState>(UGameplayStatics::GetGameState(GetWorld()));
		GameState->SetPlayerReady(DesertSlidePlayerState, !DesertSlidePlayerState->bReadyToRace);
	}
}

void ADesertSlidePlayerController::Server_SetReadyToRace_Implementation()
{
	ADesertSlidePlayerState* DesertSlidePlayerState = GetPlayerState<ADesertSlidePlayerState>();
	if (!DesertSlidePlayerState) return;
	
	ADesertSlideGameState* GameState = Cast<ADesertSlideGameState>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->SetPlayerReady(DesertSlidePlayerState, !DesertSlidePlayerState->bReadyToRace);	
}

bool ADesertSlidePlayerController::Server_SetReadyToRace_Validate()
{
	return true;
}

void ADesertSlidePlayerController::ClientReceivePlayerStates_Implementation(const TArray<FPlayerData>& PlayerReadinessStates)
{
	LocalPlayerReadinessStates = PlayerReadinessStates;
	
	for (const FPlayerData& PlayerData : PlayerReadinessStates)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClientController: %s is %s"), *PlayerData.Name, PlayerData.ReadyToRace ? *FString("ready") : *FString("not ready"));
	}
}

