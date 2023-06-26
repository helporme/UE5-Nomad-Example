// Copyright 2023 Daniel Balatskyi.

#include "SpellTargetLocationUnderCursor.h"
#include "Abilities/GameplayAbility.h"
#include "TargetDataTrace.h"


void ASpellTargetLocationUnderCursor::OnTrace(const FHitResult& OutHitResult)
{
    if (Reticle.IsValid())
    {
        Reticle->SetLocation(OutHitResult.ImpactPoint, 0);
    }
}

void ASpellTargetLocationUnderCursor::PerformTrace(FHitResult& OutHitResult) const 
{
    if (!OwningAbility) return;

    PrimaryPC->GetHitResultUnderCursorByChannel(TraceTypeQuery1, false, OutHitResult);
}

FGameplayAbilityTargetDataHandle ASpellTargetLocationUnderCursor::MakeTargetData(const FHitResult& HitResult) const
{
    return FTargetDataTrace::MakeFromHit(OwningAbility, HitResult);
}