// Copyright 2023 Daniel Balatskyi.

#include "NomadAbilitySystemGlobals.h"


void UNomadAbilitySystemGlobals::InitGlobalTags()
{
    Super::InitGlobalTags();

    SpellStageTargetingTag = FGameplayTag::RequestGameplayTag(SpellStageTargetingName, true); 
    SpellStageTriggeredTag = FGameplayTag::RequestGameplayTag(SpellStageTriggeredName, true); 
    SpellStageCastedTag = FGameplayTag::RequestGameplayTag(SpellStageCastedName, true); 
    SpellStageFinishedTag = FGameplayTag::RequestGameplayTag(SpellStageFinishedName, true); 
    SpellStageReadyTag = FGameplayTag::RequestGameplayTag(SpellStageReadyName, true); 
    SpellStageCanceledTag = FGameplayTag::RequestGameplayTag(SpellStageCanceledName, true); 
}
