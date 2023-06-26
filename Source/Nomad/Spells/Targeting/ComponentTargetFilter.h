// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetDataFilter.h"
#include "AbilitySystemComponent.h"
#include "ComponentTargetFilter.generated.h"

/**
 *  Filters actors with a specific component.
*/
USTRUCT(BlueprintType)
struct FComponentTargetFilter : public FGameplayTargetDataFilter
{
    GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Filter)
    TSubclassOf<UActorComponent> FilterComponent;

	bool FilterPassesForActor(const AActor* ActorToBeFiltered) const override
    {
        if (FilterComponent && ActorToBeFiltered 
            && ActorToBeFiltered->GetComponentByClass(FilterComponent) == nullptr)
        {
            return (bReverseFilter ^ false);
        }

        return Super::FilterPassesForActor(ActorToBeFiltered);
    }
};