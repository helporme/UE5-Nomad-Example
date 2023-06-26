// Copyright 2023 Daniel Balatskyi.

#include "NomadSpellBarWidget.h"
#include "Nomad/Spells/NomadSpell.h"
#include "Nomad/Logging.h"


bool UNomadSpellBarWidget::Initialize()
{
    SpellVisibleTag = FGameplayTag::RequestGameplayTag(SpellVisibleTagName); 
    return Super::Initialize();
}

void UNomadSpellBarWidget::BindSpellSystem(UNomadSpellSystemComponent* NewSpellSystem)
{
    if (!NewSpellSystem)
    {
        UE_LOG(LogNomad, Error, TEXT("NewSpellSystem is nullptr"))
        return;
    }

    if (SpellSystem)
    {
        // Remove old spell widgets
        for (const FGameplayAbilitySpec& SpellSpec : SpellSystem->GetActivatableAbilities())
        {
            if (UNomadSpell* Spell = Cast<UNomadSpell>(SpellSpec.Ability))
            {
                OnSpellRemoved(FSpellBindingInfo { SpellSpec.Handle, Spell, SpellSystem });
            }
        }
    }

    SpellSystem = NewSpellSystem;

    // Add new spell widgets
    for (const FGameplayAbilitySpec& SpellSpec : SpellSystem->GetActivatableAbilities())
    {
        if (UNomadSpell* Spell = Cast<UNomadSpell>(SpellSpec.Ability))
        {
            if (Spell->AbilityTags.HasTagExact(SpellVisibleTag))
            {
                OnSpellAdded(FSpellBindingInfo { SpellSpec.Handle, Spell, SpellSystem });
            }
        }
    }
}