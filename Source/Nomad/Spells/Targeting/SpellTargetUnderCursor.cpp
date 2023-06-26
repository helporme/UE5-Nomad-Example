// Copyright 2023 Daniel Balatskyi.

#include "SpellTargetUnderCursor.h"
#include "TargetDataTrace.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/PlayerController.h"


void ASpellTargetUnderCursor::Tick(float DeltaSeconds) 
{
    if (SourceActor && SourceActor->GetLocalRole() != ENetRole::ROLE_SimulatedProxy)
    {
        FHitResult HitResult;
        PerformTrace(HitResult);
		OnTrace(HitResult);

#if ENABLE_DRAW_DEBUG
		if (bDebug)
		{
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 16, 10, FColor::Green, false);
		}
#endif // ENABLE_DRAW_DEBUG
    }
}

void ASpellTargetUnderCursor::ConfirmTargetingAndContinue()
{
	check(ShouldProduceTargetData());
	if (SourceActor)
	{
		bDebug = false;

        FHitResult HitResult;
        PerformTrace(HitResult);
		FGameplayAbilityTargetDataHandle Handle = MakeTargetData(HitResult);

		TargetDataReadyDelegate.Broadcast(Handle);
	}
}
