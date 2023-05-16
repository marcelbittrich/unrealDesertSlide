// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StartFinish.generated.h"

UCLASS()
class DESERTSLIDE_API AStartFinish : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStartFinish();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerVolume;
	
	UPROPERTY()
	class UDesertSlideGameInstance* GameInstance;

	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behaviour", meta = (AllowPrivateAccess = "true"))
	bool bStart;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behaviour", meta = (AllowPrivateAccess = "true"))
	bool bFinish;
};
