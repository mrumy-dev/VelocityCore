using UnrealBuildTool;
using System.Collections.Generic;

public class VelocityCoreTarget : TargetRules
{
	public VelocityCoreTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		CppStandard = CppStandardVersion.Cpp20;
		ExtraModuleNames.Add("VelocityCore");
	}
}
