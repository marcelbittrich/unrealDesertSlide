// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostUser;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ConnectionFraction;
	
	UPROPERTY(BlueprintReadOnly)
	bool bSelected = false;
	
	void Setup(class UMainMenu* Parent, uint32 Index);

protected:
	virtual bool Initialize() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* RowButton;
	
	UFUNCTION()
	void OnClicked();
	
	UPROPERTY()
	UMainMenu* Parent;
	uint32 Index;
};
