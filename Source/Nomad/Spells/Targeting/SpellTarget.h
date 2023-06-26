// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "SpellReticle.h"
#include "SpellTarget.generated.h"


/**
 * The SpellTarget is a base class for all Nomad's spell target actors. 
 * Manages a reticle lifetime.
*/
UCLASS(Blueprintable, notplaceable)
class NOMAD_API ASpellTarget : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

	public:
		ASpellTarget();

		virtual void StartTargeting(UGameplayAbility* Ability) override;

		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	protected:
		TWeakObjectPtr<ASpellReticle> Reticle;
};
