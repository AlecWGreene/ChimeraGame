// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ChimeraGame : ModuleRules
{
	public ChimeraGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"ChimeraGAS", 
			"ChimeraInput", 
			"CoreUObject", 
			"Engine",
			"GameplayTags",
			"InputCore", 
			"HeadMountedDisplay", 
			"EnhancedInput" });
	}
}
