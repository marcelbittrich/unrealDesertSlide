// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertSlidePlayerController.h"
#include "GameFramework/Character.h"

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

		FRotator DeltaRotation =  VelocityDirection.Rotation() + FRotator(-AutoRotatePitchAngle, 0, 0) - CameraViewDirection.Rotation();
		FRotator PartialDeltaRotation = FMath::InterpEaseInOut(FRotator::ZeroRotator, DeltaRotation, AutoRotateInterpSpeed * DeltaSeconds, 1.f);

		SetControlRotation(CurrentControlRotation + PartialDeltaRotation);
	}
}
