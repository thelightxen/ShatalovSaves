// (c) XenFFly

using UnrealBuildTool;
using System.Collections.Generic;

public class ShatalovSavesTarget : TargetRules
{
	public ShatalovSavesTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ShatalovSaves" } );
	}
}
