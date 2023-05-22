// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertCharacterMovementComponent.h"

void UDesertCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	UE_LOG(LogTemp, Warning, TEXT("Desert Movement Component Initialized"));
}

void UDesertCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Warning, TEXT("Desert Moement Comp Tick"));
}


