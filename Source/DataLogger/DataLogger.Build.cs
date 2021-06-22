// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class DataLogger : ModuleRules
	{


		public DataLogger(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;


            //PrivateIncludePaths.AddRange(new string[] { "DataLogger/Private" });
            //PublicIncludePaths.AddRange(new string[] { "DataLogger/Public" });
            PrivateIncludePaths.AddRange(new string[] {

            });
            PublicIncludePaths.AddRange(new string[] {

            });

            PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Http", "Json", "JsonUtilities" });
	
            PrivateDependencyModuleNames.AddRange(
              new string[]
              {
                 "CoreUObject",
                 "Engine",
              }
            );

            DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
				);
		}
	}
}
