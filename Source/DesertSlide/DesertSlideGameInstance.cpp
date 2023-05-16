// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertSlideGameInstance.h"

UDesertSlideGameInstance::UDesertSlideGameInstance(const FObjectInitializer & ObjectInitializer)
{
	
}

void UDesertSlideGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("GameInstace Initialized"));
}