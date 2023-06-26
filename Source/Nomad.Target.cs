// Copyright 2023 Daniel Balatskyi.

using UnrealBuildTool;
using System.Collections.Generic;

public class NomadTarget : TargetRules
{
    public NomadTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest; // 5.2
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("Nomad");
    }
}
