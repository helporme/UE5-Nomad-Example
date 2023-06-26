// Copyright 2023 Daniel Balatskyi.

#include "NomadEngineSubsystem.h"
#include "Spells/NomadAbilitySystemGlobals.h"


void UNomadEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UAbilitySystemGlobals::Get().InitGlobalData();
}
