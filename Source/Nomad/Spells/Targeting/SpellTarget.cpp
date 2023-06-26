// Copyright 2023 Daniel Balatskyi.

#include "SpellTarget.h"
#include "TargetDataTrace.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/PlayerController.h"


ASpellTarget::ASpellTarget()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PostUpdateWork;
}

void ASpellTarget::StartTargeting(UGameplayAbility* InAbility)
{
    Super::StartTargeting(InAbility);
	SourceActor = InAbility->GetCurrentActorInfo()->AvatarActor.Get();

	if (ReticleClass)
	{
		ASpellReticle* SpawnedReticleActor = GetWorld()->SpawnActor<ASpellReticle>(ReticleClass, GetActorLocation(), GetActorRotation());
		if (SpawnedReticleActor)
		{
			SpawnedReticleActor->SetSourceActor(SourceActor);
			SpawnedReticleActor->InitializeReticle(this, PrimaryPC, ReticleParams);
			Reticle = SpawnedReticleActor;

			if (!ShouldProduceTargetDataOnServer)
			{
				SpawnedReticleActor->SetReplicates(false);
			}
		}
	}
}

void ASpellTarget::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (Reticle.IsValid())
    {
        Reticle.Get()->Destroy();
    }

    Super::EndPlay(EndPlayReason);
}