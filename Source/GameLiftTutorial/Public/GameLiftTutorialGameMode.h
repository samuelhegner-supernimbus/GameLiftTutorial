// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameLiftServerSDK.h"
#include "GameLiftTutorialGameMode.generated.h"


// Custom Log category
DECLARE_LOG_CATEGORY_EXTERN(GameLift, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(GameMode, Log, All);

UCLASS(minimalapi)
class AGameLiftTutorialGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameLiftTutorialGameMode();
	
protected:
	virtual void BeginPlay() override;
	
private:
	void InitGameLift();

	FGameLiftServerSDKModule* gameLiftSdkModule;
	FProcessParameters* processParameters;
	FServerParameters* serverParameters;
};



