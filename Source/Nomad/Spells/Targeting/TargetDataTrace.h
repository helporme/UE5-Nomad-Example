// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "TargetDataTrace.generated.h"

/**
 * Helper class to make the GameplayAbilityTargetDataHandle from hit results.
*/
USTRUCT(BlueprintType)
struct NOMAD_API FTargetDataTrace : public FGameplayAbilityTargetData_SingleTargetHit
{
    GENERATED_BODY()

    static FGameplayAbilityTargetDataHandle MakeFromHit(TWeakObjectPtr<UGameplayAbility> Ability, const FHitResult& HitResult)
    {
        TArray<FHitResult> HitResults;
        HitResults.Add(HitResult);
        return FTargetDataTrace::MakeFromHits(Ability, HitResults);
    };

    static FGameplayAbilityTargetDataHandle MakeFromHits(TWeakObjectPtr<UGameplayAbility> Ability, const TArray<FHitResult>& HitResults)
    {
        FGameplayAbilityTargetDataHandle ReturnDataHandle;

        for (int32 i = 0; i < HitResults.Num(); i++)
        {
            /** Note: These are cleaned up by the FGameplayAbilityTargetDataHandle (via an internal TSharedPtr) */
            FTargetDataTrace* ReturnData = new FTargetDataTrace();
            ReturnData->HitResult = HitResults[i];
            ReturnDataHandle.Add(ReturnData);
        }
        
        return ReturnDataHandle;
    };
};