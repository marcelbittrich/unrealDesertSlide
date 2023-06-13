// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuInterface.h"
#include "Engine/GameInstance.h"
#include "DesertSlideSaveGame.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "DesertSlideGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DESERTSLIDE_API UDesertSlideGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

	UDesertSlideGameInstance(const FObjectInitializer & ObjectInitializer);
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();
	
public:
	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();
	
	UFUNCTION()
	virtual void LoadSoloLevel() override;

	UFUNCTION()
	virtual void GoToMainMenu() override;
	
	UFUNCTION()
	virtual void QuitGame() override;
	
	UFUNCTION()
	virtual void RestartLevel() override;

	UFUNCTION()
	virtual void StartRace() override;

	UFUNCTION()
	virtual void HandlePlayerReadyChange(bool bReady) override;
	
	UFUNCTION()
    virtual void HandleFollowCamChange(bool bFollowCam) override;

	UFUNCTION()
	FORCEINLINE UDesertSlideSaveGame* GetSaveGame() const {return SaveGame;}

	UFUNCTION()
	void LoadMapSaveGame();
	UFUNCTION()
	void WriteMapSaveGame();

	UFUNCTION()
	virtual void RefreshServerList() override;
	
	UFUNCTION(Exec)
	virtual void Host(FString& ServerName) override;
	
	UFUNCTION(Exec)
	virtual void Join(uint32 Index) override;

	void StartSession();

private:
	void LoadSaveGame(const FString& MapName);
	void WriteSaveGame(const FString& MapName);

	UPROPERTY()
	TSubclassOf<class UUserWidget> MainMenuClass;
	UPROPERTY()
	class UMainMenu* MainMenu;
	
	UPROPERTY()
	TSubclassOf<class UUserWidget> InGameMenuClass;
	UPROPERTY()
	class UMenuWidget* InGameMenu;

	UPROPERTY()
	class UDesertSlideSaveGame* SaveGame;

	int UserIndex = 1;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	UPROPERTY()
	FString NewServerName;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void CreateSession();
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver , ENetworkFailure::Type FailureType, const FString& ErrorString);
};
