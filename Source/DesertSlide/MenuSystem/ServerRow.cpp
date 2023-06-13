// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/ServerRow.h"

#include "MainMenu.h"
#include "Components/Button.h"

void UServerRow::Setup(UMainMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
}

bool UServerRow::Initialize()
{
	bool Success =  Super::Initialize();
	if (!Success) return false;

	if (!RowButton) return false;
	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
	
	return true;
}

void UServerRow::OnClicked()
{
	Parent->SelectIndex(Index);
}

