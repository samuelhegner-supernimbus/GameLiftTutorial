// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameLiftServerSDK.h"
#include "BrainCloudS2S.h"
#include "GameFramework/GameModeBase.h"
#include "GameLiftTutorialGameMode.generated.h"


// Custom Log categories
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
	// GameModeBase override functions we will use
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

private:
	// Initialisation functions
	void InitGameLift();
	void InitBrainCloud();

	// Shutdown functions
	void ShutDownGameLift() const;
	void ShutDownBrainCloud() const;
	void ShutDownServer(const FString& reason) const;

	// Empty server timer functions
	void StartTimer();
	void CheckDisconnectedPlayers();
	void EmptyServerTimeout() const;

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

	// BrainCloud sdk reference
	TSharedPtr<UBrainCloudS2S> pS2S;

	// Flag to start running BC s2s callbacks
	bool brainCloudReady = false;

	// Player tracking
	int playerCount = 0;
	int secondsToShutDownEmptyServer = 10;
	int timerCounter = 0;
	FTimerHandle ShutdownTimer;
};
