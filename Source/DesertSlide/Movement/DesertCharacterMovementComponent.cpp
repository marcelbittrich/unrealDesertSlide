// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertCharacterMovementComponent.h"

#include "GameFramework/Character.h"

bool UDesertCharacterMovementComponent::FSavedMove_Desert::CanCombineWith(const FSavedMovePtr& NewMove,
                                                                          ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_Desert* NewDesertMove = static_cast<FSavedMove_Desert*>(NewMove.Get());

	// If you start to sprint, you dont want to combine the movements.
	if (Saved_bWantsToSprint != NewDesertMove->Saved_bWantsToSprint)
	{
		return false;
	}
	
	// In case we want to combine, the super function will handle that.
	return Super::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UDesertCharacterMovementComponent::FSavedMove_Desert::Clear()
{
	Super::Clear();

	Saved_bWantsToSprint = 0;
}

uint8 UDesertCharacterMovementComponent::FSavedMove_Desert::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (Saved_bWantsToSprint)
	{
		Result |= FLAG_Custom_0;
	}

	return Result;
}

void UDesertCharacterMovementComponent::FSavedMove_Desert::SetMoveFor(ACharacter* C, float InDeltaTime,
	FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UDesertCharacterMovementComponent* CharacterMovement = Cast<UDesertCharacterMovementComponent>(C->GetCharacterMovement());

	Saved_bWantsToSprint = CharacterMovement->Safe_bWantsToSprint;
}

void UDesertCharacterMovementComponent::FSavedMove_Desert::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);
	
	UDesertCharacterMovementComponent* CharacterMovement = Cast<UDesertCharacterMovementComponent>(C->GetCharacterMovement());

	CharacterMovement->Safe_bWantsToSprint = Saved_bWantsToSprint;
}

UDesertCharacterMovementComponent::FNetworkPredictionData_Client_Desert::FNetworkPredictionData_Client_Desert(
	const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{
	// Just calling the Super constructor.
}

FSavedMovePtr UDesertCharacterMovementComponent::FNetworkPredictionData_Client_Desert::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Desert());
}

FNetworkPredictionData_Client* UDesertCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr)

	if (ClientPredictionData == nullptr)
	{
		// To create the prediction data if not available, it has to const_cast away the const of the component,
		// since this is a const function.
		UDesertCharacterMovementComponent* MutableThis = const_cast<UDesertCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Desert(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}
	
	return ClientPredictionData;
}

void UDesertCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	
	Safe_bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UDesertCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (MovementMode == MOVE_Walking)
	{
		if (Safe_bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
	
}

UDesertCharacterMovementComponent::UDesertCharacterMovementComponent()
{
	
}

void UDesertCharacterMovementComponent::SprintPressed()
{
	Safe_bWantsToSprint = true;
}

void UDesertCharacterMovementComponent::SprintReleased()
{
	Safe_bWantsToSprint = false;
}

