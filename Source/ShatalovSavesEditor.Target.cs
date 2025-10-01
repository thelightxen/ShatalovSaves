// (c) XenFFly

using UnrealBuildTool;
using System.Collections.Generic;

public class ShatalovSavesEditorTarget : TargetRules
{
	public ShatalovSavesEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ShatalovSaves" } );
	}
}
