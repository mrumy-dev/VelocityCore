using UnrealBuildTool;
using System.Collections.Generic;

public class VelocityCoreEditorTarget : TargetRules
{
	public VelocityCoreEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("VelocityCore");
	}
}
