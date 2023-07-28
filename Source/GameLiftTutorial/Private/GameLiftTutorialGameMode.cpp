#include "GameLiftTutorialGameMode.h"

#include "GameLiftServerSDK.h"
#include "GameLiftTutorialCharacter.h"
#include "UObject/ConstructorHelpers.h"

// Custom Log category
DEFINE_LOG_CATEGORY(GameLift);
DEFINE_LOG_CATEGORY(GameMode);
DEFINE_LOG_CATEGORY(BrainCloud);

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
	StartTimer();
#endif
}


void AGameLiftTutorialGameMode::InitGameLift()
{
	UE_LOG(GameLift, Log, TEXT("InitGameLift()"))

	serverParameters = new FServerParameters();

	FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), serverParameters->m_authToken);
	FParse::Value(FCommandLine::Get(), TEXT("-hostid="), serverParameters->m_hostId);
	FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), serverParameters->m_fleetId);
	FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), serverParameters->m_webSocketUrl);

	serverParameters->m_processId = FString::Printf(TEXT("%d"), GetCurrentProcessId());

	UE_LOG(GameLift, Log, TEXT("Server Parameters: "));
	UE_LOG(GameLift, Log, TEXT("------------------------------------------------"));
	UE_LOG(GameLift, Log, TEXT("Host Id: %s"), *serverParameters->m_hostId);
	UE_LOG(GameLift, Log, TEXT("Fleet Id: %s"), *serverParameters->m_fleetId);
	UE_LOG(GameLift, Log, TEXT("Process Id: %s"), *serverParameters->m_processId);
	UE_LOG(GameLift, Log, TEXT("Web Socket Url: %s"), *serverParameters->m_webSocketUrl);
	UE_LOG(GameLift, Log, TEXT("Auth Token: %s"), *serverParameters->m_authToken);
	UE_LOG(GameLift, Log, TEXT("------------------------------------------------"));

	UE_LOG(GameLift, Log, TEXT("Initializing the GameLift Server..."));
	gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
	const FGameLiftGenericOutcome initSdkOutcome = gameLiftSdkModule->InitSDK(*serverParameters);

	if (initSdkOutcome.IsSuccess())
	{
		UE_LOG(GameLift, Log, TEXT("InitSDK succeeded"));
	}
	else
	{
		UE_LOG(GameLift, Log, TEXT("ERROR: InitSDK failed"));
		const FGameLiftError gameLiftError = initSdkOutcome.GetError();
		UE_LOG(GameLift, Log, TEXT("ERROR: %s"), *gameLiftError.m_errorMessage);
		return;
	}

	// Define functions to handle GameLift events: OnStartGameSession, OnTerminate, OnHealthCheck
	auto onGameSession = [=](Aws::GameLift::Server::Model::GameSession gameSession)
	{
		gameSessionId = FString(gameSession.GetGameSessionId());
		UE_LOG(GameLift, Log, TEXT("GameSession Initializing: %s..."), *gameSessionId);

		int count;
		const auto props = gameSession.GetGameProperties(count);

		for (int i = 0; i < count; i++)
		{
			auto prop = props[i];
			FString key = FString::Printf(TEXT("%hs"), prop.GetKey());
			FString val = FString::Printf(TEXT("%hs"), prop.GetValue());

			if (key == "SERVER_PORT")
			{
				serverPort = val;
			}
			else if (key == "APP_ID")
			{
				appId = val;
			}
			else if (key == "LOBBY_ID")
			{
				lobbyId = val;
			}
			else if (key == "SERVER_SECRET")
			{
				serverSecret = val;
			}
			else if (key == "SERVER_NAME")
			{
				serverName = val;
			}
			else if (key == "SERVER_HOST")
			{
				serverHost = val;
			}
			else
			{
				UE_LOG(GameLift, Warning, TEXT("Gamelift - Unused GameProperty - %s:%s"), *key, *val);
			}
		}

		UE_LOG(GameLift, Warning, TEXT("Important Game Properties -------------------------------------"));
		UE_LOG(GameLift, Warning, TEXT("appId - %s"), *appId);
		UE_LOG(GameLift, Warning, TEXT("lobbyId - %s"), *lobbyId);
		UE_LOG(GameLift, Warning, TEXT("serverName - %s"), *serverName);
		UE_LOG(GameLift, Warning, TEXT("serverSecret - %s"), *serverSecret);
		UE_LOG(GameLift, Warning, TEXT("serverHost - %s"), *serverHost);
		UE_LOG(GameLift, Warning, TEXT("serverPort - %s"), *serverPort);

		InitBrainCloud();
		gameLiftSdkModule->ActivateGameSession();
		gameSessionStarted = true;
	};

	auto onTerminate = [=]()
	{
		UE_LOG(GameLift, Log, TEXT("Game Server Process is terminating..."));
		ShutDownBrainCloud();
		ShutDownGameLift();
		ShutDownServer("Termination was requested by GameLift. Shutting down server...");
	};

	auto onHealthCheck = []()
	{
		UE_LOG(GameLift, Log, TEXT("Performing Health Check..."));
		return true;
	};

	// Define log file location
	TArray<FString> logfiles;
	logfiles.Add(TEXT("GameLiftTutorial/Saved/Logs/GameLiftTutorial"));

	// Define server port
	const auto worldPort = GetWorld()->URL.Port;
	UE_LOG(GameLift, Log, TEXT("World Port: %d"), worldPort);

	// Assign process parameters
	processParameters = new FProcessParameters();
	processParameters->OnStartGameSession.BindLambda(onGameSession);
	processParameters->OnTerminate.BindLambda(onTerminate);
	processParameters->OnHealthCheck.BindLambda(onHealthCheck);
	processParameters->port = worldPort;
	processParameters->logParameters = logfiles;

	CallGameLiftProcessReady();

	UE_LOG(GameLift, Log, TEXT("Finished initialising GameLift"));
}

void AGameLiftTutorialGameMode::CallGameLiftProcessReady()
{
	UE_LOG(GameLift, Log, TEXT("Calling Process Ready..."));
	FGameLiftGenericOutcome processReadyOutcome = gameLiftSdkModule->ProcessReady(*processParameters);
	if (processReadyOutcome.IsSuccess())
	{
		UE_LOG(GameLift, Log, TEXT("Process Ready Succeded"));
	}
	else
	{
		UE_LOG(GameLift, Log, TEXT("ERROR: Process Ready Failed"));
		const auto processReadyError = processReadyOutcome.GetError();
		UE_LOG(GameLift, Log, TEXT("ERROR: %s"), *processReadyError.m_errorMessage);
	}
}

void AGameLiftTutorialGameMode::InitBrainCloud()
{
	UE_LOG(BrainCloud, Log, TEXT("InitBrainCloud()"))

	if (GetNetMode() != NM_DedicatedServer) return;

	if (brainCloudReady == false)
	{
		pS2S = MakeShareable(new UBrainCloudS2S(appId, serverName, serverSecret, "https://api.braincloudservers.com/s2sdispatcher", true));
		pS2S->setLogEnabled(true);
	}

	brainCloudReady = true;

	pS2S->authenticate();

	pS2S->request(
		"{\"service\":\"lobby\",\"operation\":\"GET_LOBBY_DATA\",\"data\":{\"lobbyId\":\"" + lobbyId + "\"}}", [this](const FString& result)
		{
			UE_LOG(BrainCloud, Warning, TEXT("BrainCloud - GET_LOBBY_DATA - %s"), *result);
			pS2S->request("{\"service\":\"lobby\",\"operation\":\"SYS_ROOM_READY\",\"data\":{\"lobbyId\":\"" + lobbyId + "\"}}", nullptr);
		});
}

void AGameLiftTutorialGameMode::ShutDownGameLift()
{
	if(gameSessionStarted == false) return;
	
	gameLiftSdkModule->ProcessEnding();
}

void AGameLiftTutorialGameMode::ShutDownBrainCloud()
{
	if (brainCloudReady == false) return;

	pS2S->request("{\"service\":\"lobby\",\"operation\":\"SYS_ROOM_STOPPED\",\"data\":{\"lobbyId\":\"" + lobbyId + "\"}}", nullptr);
}

void AGameLiftTutorialGameMode::ShutDownServer(const FString& reason)
{
	RequestEngineExit(reason);
}

void AGameLiftTutorialGameMode::StartTimer()
{
	UE_LOG(GameMode, Log, TEXT("Starting shutdown timer..."));
	GetWorldTimerManager().SetTimer(ShutdownTimer, this, &AGameLiftTutorialGameMode::CheckDisconnectedPlayers, 1.0f, true);
}

void AGameLiftTutorialGameMode::CheckDisconnectedPlayers()
{
	if(gameSessionStarted == false) return;
	
	if (playerCount <= 0)
	{
		if (timerCounter >= secondsToShutDownEmptyServer)
		{
			ShutDownGameLift();
			ShutDownBrainCloud();
			ShutDownServer("All players disconnected. GameSession is complete. Shutting down server...");
		}
		UE_LOG(GameMode, Log, TEXT("No players connected, shutting down game session in: %d"), secondsToShutDownEmptyServer-timerCounter);
		timerCounter++;
	}
	else
	{
		timerCounter = 0;
	}
}

void AGameLiftTutorialGameMode::Tick(float DeltaSeconds)
{
	// We need to run the Server to server callbacks with the BrainCloud sdk
	Super::Tick(DeltaSeconds);
#if WITH_GAMELIFT

	if (brainCloudReady == false) return;

	if (pS2S.IsValid()) pS2S->runCallbacks();
#endif
}

void AGameLiftTutorialGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	playerCount++;
}

void AGameLiftTutorialGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	playerCount--;
}
