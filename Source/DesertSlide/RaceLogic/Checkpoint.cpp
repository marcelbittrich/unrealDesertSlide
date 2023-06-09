// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint.h"

#include "DesertSlideGameInstance.h"
#include "RaceManagerSubsystem.h"
#include "Components/BoxComponent.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("Trigger Area"));
    if (!TriggerVolume) return;
	RootComponent = TriggerVolume;
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnTriggerOverlap);
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UDesertSlideGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Checkpoint did not find GameInstance"));
	}
}

void ACheckpoint::OnTriggerOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (GameInstance)
	{
		URaceManagerSubsystem* RaceManager = GameInstance->GetSubsystem<URaceManagerSubsystem>();
		if (RaceManager)
		{
			RaceManager->CheckpointCrossed(this, OtherActor);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Checkpoint did not find RaceManager"));
		}
	}
	
}

