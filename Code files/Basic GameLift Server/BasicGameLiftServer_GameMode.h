#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameLiftServerSDK.h"
#include "GameLiftTutorialGameMode.generated.h"


// Custom Log categories
DECLARE_LOG_CATEGORY_EXTERN(GameLift, Log, All);

DECLARE_LOG_CATEGORY_EXTERN(GameMode, Log, All);

UCLASS(minimalapi)
class AGameLiftTutorialGameMode : public AGameModeBase
{
   GENERATED_BODY()

public:
   AGameLiftTutorialGameMode();

protected:
   // GameModeBase override functions
   virtual void BeginPlay() override;

private:
   // Initialisation functions
   void InitGameLift();

   FGameLiftServerSDKModule* gameLiftSdkModule;
   FProcessParameters* processParameters;
   FServerParameters* serverParameters;
};