// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameJam23 : ModuleRules
{
	public GameJam23(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "NavigationSystem","UMG" });
	}
}
