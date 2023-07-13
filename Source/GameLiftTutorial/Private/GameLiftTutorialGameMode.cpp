
#include "GameLiftTutorialGameMode.h"

#include "GameLiftServerSDK.h"
#include "GameLiftTutorialCharacter.h"
#include "UObject/ConstructorHelpers.h"

// Custom Log category
DEFINE_LOG_CATEGORY(GameLift);
DEFINE_LOG_CATEGORY(GameMode);

AGameLiftTutorialGameMode::AGameLiftTutorialGameMode()
{
	UE_LOG(GameMode, Log, TEXT("AGameLiftTutorialGameMode()"));
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	UE_LOG(GameMode, Log, TEXT("Enabling ticking..."));
	PrimaryActorTick.bCanEverTick = true;
}

void AGameLiftTutorialGameMode::BeginPlay()
{
	UE_LOG(GameMode, Log, TEXT("BeginPlay()"));
	Super::BeginPlay();
	
#if WITH_GAMELIFT
	InitGameLift();
#endif
}

void AGameLiftTutorialGameMode::InitGameLift()
{
	UE_LOG(GameLift, Warning, TEXT("InitGameLift()"));
	gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
	
	//Call InitSDK to establish a local connection with the GameLift agent to enable further communication.
	UE_LOG(GameLift, Warning, TEXT("Initialising the SDK..."));
	const auto initSdkOutcome = gameLiftSdkModule->InitSDK(*serverParameters);

	if(initSdkOutcome.IsSuccess())
	{
		UE_LOG(GameLift, Warning, TEXT("Initialising the SDK was successful"));

		params = new FProcessParameters();

		// Basic Game session started function
		auto onGameSession = [=](Aws::GameLift::Server::Model::GameSession gameSession)
		{
			UE_LOG(GameLift, Warning, TEXT("Game session is initialising..."));
			gameLiftSdkModule->ActivateGameSession();
			UE_LOG(GameLift, Warning, TEXT("Game session is initialised sucessfully and session is active"));
		};

		// Basic termination function
		auto onTerminate = [=]()
		{
			UE_LOG(GameLift, Warning, TEXT("Game Server Process is terminating..."));
			ShutDownGameLift();
		};

		// Basic health check function
		auto onHealthCheck = []()
		{
			UE_LOG(GameLift, Warning, TEXT("Performing Health Check..."));
			return true;
		};

		// Basic logParameters setup
		TArray<FString> logfiles;
		logfiles.Add(TEXT("aLogFile.txt"));

		// Configuring Process Parameters
		params->OnStartGameSession.BindLambda(onGameSession);
		params->OnTerminate.BindLambda(onTerminate);
		params->OnHealthCheck.BindLambda(onHealthCheck);
		params->port = 7777;
		params->logParameters = logfiles;

		UE_LOG(GameLift, Warning, TEXT("Calling Process Ready..."));
		const auto readyOutcome = gameLiftSdkModule->ProcessReady(*params);

		if (readyOutcome.IsSuccess())
		{
			UE_LOG(GameLift, Warning, TEXT("Process successfully readied up. Ready for players to join"));
		}
		else
		{
			UE_LOG(GameLift, Error, TEXT("Unable to ready up the process. An error occurred"));
		}
	}
}

void AGameLiftTutorialGameMode::ShutDownGameLift()
{
	gameLiftSdkModule->ProcessEnding();
}
