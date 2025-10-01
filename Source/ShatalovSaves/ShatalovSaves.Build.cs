// (c) XenFFly

using UnrealBuildTool;

public class ShatalovSaves : ModuleRules
{
	public ShatalovSaves(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "JsonUtilities", "Json" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
