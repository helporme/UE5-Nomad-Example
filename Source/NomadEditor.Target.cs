// Copyright 2023 Daniel Balatskyi.

using UnrealBuildTool;
using System.Collections.Generic;

public class NomadEditorTarget : TargetRules
{
	public NomadEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest; // 5.2 
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("Nomad");
	}
}
