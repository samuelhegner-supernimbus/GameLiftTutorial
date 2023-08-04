// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using Internal;
using UnrealBuildTool;

public class GameLiftTutorial : ModuleRules
{
	public GameLiftTutorial(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"HeadMountedDisplay",
			"EnhancedInput",
			"GameLiftServerSDK"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"JsonUtilities",
			"HTTP",
			"Json"
		});

		const string outputBinDir = "$(TargetOutputDir)";
		const string gameLiftFilesDir = "$(ProjectDir)/GameLiftFiles";
		RuntimeDependencies.Add($"{outputBinDir}/libcrypto-3-x64.dll", $"{gameLiftFilesDir}/libcrypto-3-x64.dll");
		RuntimeDependencies.Add($"{outputBinDir}/libssl-3-x64.dll", $"{gameLiftFilesDir}/libssl-3-x64.dll");
	}
}