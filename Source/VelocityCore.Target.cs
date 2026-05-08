using UnrealBuildTool;
using System.Collections.Generic;

public class VelocityCoreTarget : TargetRules
{
	public VelocityCoreTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("VelocityCore");
	}
}
