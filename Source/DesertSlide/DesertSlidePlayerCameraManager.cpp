// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertSlidePlayerCameraManager.h"

#include "DesertSlideCharacter.h"
#include "Movement/DesertCharacterMovementComponent.h"

ADesertSlidePlayerCameraManager::ADesertSlidePlayerCameraManager()
{
	
}

void ADesertSlidePlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	ADesertSlideCharacter* DesertSlideCharacter = Cast<ADesertSlideCharacter>(GetOwningPlayerController()->GetPawn());
	if (DesertSlideCharacter)
	{
		UDesertCharacterMovementComponent* DesertMovementComponent = DesertSlideCharacter->GetDesertCharacterMovement();
		
		float HeightOffset = DesertMovementComponent->GetCrouchedHalfHeight() - DesertSlideCharacter->GetDefaultHalfHeight();
		FVector TargetCrouchOffset = FVector(0, 0, HeightOffset);

		float Blend = FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f);
		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, Blend);

		if (DesertMovementComponent->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			
			// When crouching camera VT is set automatically to lower position.
			// The following gets it back up so we can smoothly transition down.
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
		}

		if(DesertMovementComponent->IsMovingOnGround())
		{
			OutVT.POV.Location += Offset;
		}
	}
}