using UnrealBuildTool;
using System.Collections.Generic;

public class VelocityCoreEditorTarget : TargetRules
{
	public VelocityCoreEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		CppStandard = CppStandardVersion.Cpp20;
		ExtraModuleNames.Add("VelocityCore");
	}
}
