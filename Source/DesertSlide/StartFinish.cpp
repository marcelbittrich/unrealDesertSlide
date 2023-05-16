// Fill out your copyright notice in the Description page of Project Settings.


#include "StartFinish.h"

#include "DesertSlideGameInstance.h"
#include "RaceManagerSubsystem.h"
#include "Components/BoxComponent.h"

// Sets default values
AStartFinish::AStartFinish()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("Trigger Area"));
	if (!TriggerVolume) return;
	
	RootComponent = TriggerVolume;
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AStartFinish::OnTriggerOverlap);
}

// Called when the game starts or when spawned
void AStartFinish::BeginPlay()
{
	Super::BeginPlay();
	
	GameInstance = Cast<UDesertSlideGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartFinish did not find GameInstance"));
	}
}

void AStartFinish::OnTriggerOverlap(
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
			if(bStart)
			{
				RaceManager->StartCrossed(OtherActor);
			}
			if(bFinish)
			{
				RaceManager->FinishCrossed(OtherActor);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("StartFinish did not find RaceManager"));
		}
	}
}

