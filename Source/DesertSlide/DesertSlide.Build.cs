// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DesertSlide : ModuleRules
{
	public DesertSlide(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "OnlineSubsystem", "OnlineSubsystemSteam" });

		PublicIncludePaths.AddRange(
			new string[]
			{
				"DesertSlide",
				"DesertSlide/UI",
				"DesertSlide/RaceLogic",
				"DesertSlide/MenuSystem"
			}
		);

	}
}
