// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameLiftServerSDK.h"
#include "BrainCloudS2S.h"
#include "GameFramework/GameModeBase.h"
#include "GameLiftTutorialGameMode.generated.h"


// Custom Log category
DECLARE_LOG_CATEGORY_EXTERN(GameLift, Log, All);

DECLARE_LOG_CATEGORY_EXTERN(BrainCloud, Log, All);

DECLARE_LOG_CATEGORY_EXTERN(GameMode, Log, All);

UCLASS(minimalapi)
class AGameLiftTutorialGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameLiftTutorialGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

private:
	void InitGameLift();
	void CallGameLiftProcessReady();
	void InitBrainCloud();

	void ShutDownGameLift();
	void ShutDownBrainCloud();
	void ShutDownServer(const FString& reason);

	void StartTimer();
	void CheckDisconnectedPlayers();

	// GameLift sdk reference and parameters that the sdk is initialised with
	FGameLiftServerSDKModule* gameLiftSdkModule;
	FProcessParameters* processParameters;
	FServerParameters* serverParameters;

	// GameLift Session Info
	bool gameSessionStarted = false;
	FString gameSessionId = "";

	// BrainCloud info provided at server launch
	FString appId = "";
	FString serverHost = "";
	FString serverPort = "";
	FString serverName = "";
	FString serverSecret = "";
	FString lobbyId = "";

	// Flag to start running BC s2s callbacks
	bool brainCloudReady = false;

	// BrainCloud sdk reference
	TSharedPtr<UBrainCloudS2S> pS2S;

	// GameSession Tracking
	int playerCount = 0;
	int secondsToShutDownEmptyServer = 10;
	int timerCounter = 0;
	FTimerHandle ShutdownTimer;
};
