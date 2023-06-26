// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "SpellTarget.h"
#include "SpellTargetUnderCursor.generated.h"


/**
 * The SpellTargetUnderCursor is a base abstract class for all cursor-based targeting. 
*/
UCLASS(Blueprintable, Abstract, notplaceable)
class NOMAD_API ASpellTargetUnderCursor : public ASpellTarget
{
	GENERATED_BODY()
	
	public:
		virtual void Tick(float DeltaSeconds) override;

		virtual void ConfirmTargetingAndContinue() override;

	protected:
		virtual void OnTrace(const FHitResult& HitResult) { };

		virtual void PerformTrace(FHitResult& OutHitResult) const PURE_VIRTUAL(ASpellTargetUnderCursor::PerformTrace, );;

		virtual FGameplayAbilityTargetDataHandle MakeTargetData(const FHitResult& HitResult) const 
			PURE_VIRTUAL(ASpellTargetUnderCursor::MakeTargetData, return FGameplayAbilityTargetDataHandle { }; );
};