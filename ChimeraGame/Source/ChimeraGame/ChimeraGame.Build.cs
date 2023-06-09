// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ChimeraGame : ModuleRules
{
	public ChimeraGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
