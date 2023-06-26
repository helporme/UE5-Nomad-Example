// Copyright 2023 Daniel Balatskyi.

#include "NomadSpellSystemComponent.h"


void UNomadSpellSystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) 
{
    Super::OnGiveAbility(AbilitySpec);
    OnAbilityGiven.Broadcast(AbilitySpec.Handle);
}

void UNomadSpellSystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) 
{
    Super::OnRemoveAbility(AbilitySpec);
    OnAbilityRemoved.Broadcast(AbilitySpec.Handle);
}