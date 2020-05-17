// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class WVModule : ModuleRules
{
	public WVModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        //The path for the header files
		PublicIncludePaths.AddRange(new string[] { "WVModule/Public" });
 
		//The path for the source files
		PrivateIncludePaths.AddRange(new string[] { "WVModule/Private" });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
