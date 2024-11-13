// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class shadowTest : ModuleRules
{
	public shadowTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Niagara", "LevelSequence", "MovieScene" });
	}
}
