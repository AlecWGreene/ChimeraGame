// Copyright Alec Greene. All Rights Reserved.

using UnrealBuildTool;

public class ChimeraGAS_BlueprintNodes : ModuleRules
{
	public ChimeraGAS_BlueprintNodes(ReadOnlyTargetRules Target) : base(Target)
	{

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"AIModule",
				"BlueprintGraph",
				"Core",
				"CoreUObject",
				"ChimeraGAS",
				"Engine",
				"EnhancedInput",
				"GameplayAbilities",
				"GameplayTags",
				"GameplayTasks",
				"GraphEditor",
				"KismetCompiler",
				"PropertyEditor",
				"Slate",
				"SlateCore",
				"UnrealEd"
			}
			);
	}
}