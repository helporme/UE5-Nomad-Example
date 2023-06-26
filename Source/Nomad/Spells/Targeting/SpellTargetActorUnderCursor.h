// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "SpellTargetLocationUnderCursor.h"
#include "SpellTargetActorUnderCursor.generated.h"


/**
 * Targets an actor under user pointer.
*/
UCLASS(Blueprintable, notplaceable)
class NOMAD_API ASpellTargetActorUnderCursor : public ASpellTargetLocationUnderCursor
{
	GENERATED_BODY()

	public:
		void OnTrace(const FHitResult& HitResult) override;

		virtual FGameplayAbilityTargetDataHandle MakeTargetData(const FHitResult& HitResult) const override;
};
