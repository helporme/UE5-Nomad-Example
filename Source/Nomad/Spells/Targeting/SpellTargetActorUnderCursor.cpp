// Copyright 2023 Daniel Balatskyi.

#include "SpellTargetActorUnderCursor.h"
#include "TargetDataTraceActor.h"


void ASpellTargetActorUnderCursor::OnTrace(const FHitResult& HitResult)
{
    AActor* Victim = HitResult.GetActor();

    if (Reticle.IsValid())
    {
        if (Victim) 
        {
            Reticle->SetVictim(Victim, Filter.FilterPassesForActor(Victim));
        }
    }
}

FGameplayAbilityTargetDataHandle ASpellTargetActorUnderCursor::MakeTargetData(const FHitResult& HitResult) const
{
    AActor* Victim = HitResult.GetActor();

    TArray<TWeakObjectPtr<AActor>> Victims;
    if (Filter.FilterPassesForActor(Victim))
    {
        Victims.Add(HitResult.GetActor());
    }

    return FTargetDataTraceActor::MakeTargetDataHandleFromActors(Victims, true);
}