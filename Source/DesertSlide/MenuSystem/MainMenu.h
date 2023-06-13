// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OptionsMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "MainMenu.generated.h"


class UComboBoxString;
class UButton;
class UGameUserSettings;

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint8 CurrentPlayers;
	uint8 MaxPlayers;
	FString HostUserName;
};

UCLASS()
class DESERTSLIDE_API UMainMenu : public UOptionsMenu
{
	GENERATED_BODY()
	
public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual bool Initialize() override;

private:

	UPROPERTY(meta = (BindWidget)) 
	class UWidgetSwitcher* MenuSwitcher;

	// Menus
	UPROPERTY(meta = (BindWidget))
	class UWidget* Selector;
	UPROPERTY(meta = (BindWidget))
	class UWidget* Options;
	UPROPERTY(meta = (BindWidget))
	class UWidget* Warning;
	UPROPERTY(meta = (BindWidget))
	class UWidget* MultiplayerMenu;
	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;
	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;
	
	// Selector
	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;
	UPROPERTY(meta = (BindWidget))
	UButton* SoloButton;
	UPROPERTY(meta = (BindWidget))
	UButton* MultiplayerButton;
	UPROPERTY(meta = (BindWidget))
	UButton* OptionsButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	// Options
	UPROPERTY(meta = (BindWidget))
	UButton* ApplySettingsButton;

	// Warning
	UPROPERTY(meta = (BindWidget))
	UButton* BackWarningButton;
	UPROPERTY(meta = (BindWidget))
	UButton* GotItWarningButton;

	// Multiplayer Menu
	UPROPERTY(meta = (BindWidget))
	UButton* JoinMultiplayerButton;
	UPROPERTY(meta = (BindWidget))
	UButton* HostMultiplayerButton;
	UPROPERTY(meta = (BindWidget))
	UButton* BackMultiplayerButton;

	// Host Menu
	UPROPERTY(meta = (BindWidget))
	UButton* CancelHostMenuButton;
	UPROPERTY(meta = (BindWidget))
	UButton* HostHostMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerNameTextBox;

	// Join Menu
	UPROPERTY(meta = (BindWidget))
	UButton* CancelJoinMenuButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ConnectJoinMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SearchingText;

	// Init
	bool InitSelector();
	bool InitOptions();
	bool InitWarning();
	bool InitMultiplayerMenu();
	bool InitHostMenu();
	bool InitJoinMenu();

	// Open
	UFUNCTION()
	void OpenSelector();
	UFUNCTION()
	void OpenOptionsMenu();
	UFUNCTION()
	void OpenWarning();
	UFUNCTION()
	void OpenMultiplayerMenu();
	UFUNCTION()
	void OpenHostMenu();
	UFUNCTION()
	void OpenJoinMenu();
	
	
	UFUNCTION()
	void StartSoloGame();

	// Multiplayer functions
public:
	void SetServerList(TArray<FServerData>& ServerNames);
	void SelectIndex(uint32 Index);
	
private:
	UFUNCTION()
	void HostServer();
	UFUNCTION()
	void JoinServer();

	UPROPERTY()
	TSubclassOf<class UServerRow> ServerRowClass;
	
	UPROPERTY()
	class UServerRow* ServerRow;
	UPROPERTY(meta = (BindWidget)) 
	class UScrollBox* ServerList;
	
	TOptional<uint32> SelectedIndex;
	void UpdateServerRows();

	// Options functions
	
	void ReadCurrentSettings();
	UFUNCTION()
	void ApplySettings();
	
	UFUNCTION()
	void QuitGame();
};
