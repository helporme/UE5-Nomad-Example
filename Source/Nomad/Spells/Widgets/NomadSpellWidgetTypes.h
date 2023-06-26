// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "Nomad/Spells/NomadSpell.h"
#include "Nomad/Spells/NomadSpellSystemComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "NomadSpellWidgetTypes.generated.h"


/**
 * The SpellBindingInfo is passed from the NomadSpellBarWidget to the NomadSpellWidget.  
*/
USTRUCT(BlueprintType)
struct FSpellBindingInfo 
{	
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Nomad|Spell")
    FGameplayAbilitySpecHandle SpellHandle;

	UPROPERTY(BlueprintReadOnly, Category="Nomad|Spell")
    TObjectPtr<UNomadSpell> Spell;
	
	UPROPERTY(BlueprintReadOnly, Category="Nomad|Spell")
	TObjectPtr<UNomadSpellSystemComponent> SpellSystem;
};