// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "SpellTargetUnderCursor.h"
#include "SpellTargetLocationUnderCursor.generated.h"


/**
 * Targets a location under user pointer.
*/
UCLASS(Blueprintable, notplaceable)
class NOMAD_API ASpellTargetLocationUnderCursor : public ASpellTargetUnderCursor
{
	GENERATED_BODY()

	protected:
		virtual void OnTrace(const FHitResult& HitResult) override;

		virtual void PerformTrace(FHitResult& OutHitResult) const override;

		virtual FGameplayAbilityTargetDataHandle MakeTargetData(const FHitResult& HitResult) const override;
};
