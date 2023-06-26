// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "TargetDataTraceActor.generated.h"


/**
 * Helper class to make the GameplayAbilityTargetDataHandle from actors.
*/
USTRUCT(BlueprintType)
struct NOMAD_API FTargetDataTraceActor : public FGameplayAbilityTargetData_ActorArray
{
    GENERATED_BODY()

    static FGameplayAbilityTargetDataHandle MakeTargetDataHandleFromActors(const TArray<TWeakObjectPtr<AActor>>& TargetActors, bool OneActorPerHandle) 
    {
        /** Note: This is cleaned up by the FGameplayAbilityTargetDataHandle (via an internal TSharedPtr) */
        FTargetDataTraceActor* ReturnData = new FTargetDataTraceActor();
        FGameplayAbilityTargetDataHandle ReturnDataHandle = FGameplayAbilityTargetDataHandle(ReturnData);

        if (OneActorPerHandle)
        {
            if (TargetActors.Num() > 0)
            {
                if (AActor* TargetActor = TargetActors[0].Get())
                {
                    ReturnData->TargetActorArray.Add(TargetActor);
                }

                for (int32 i = 1; i < TargetActors.Num(); ++i)
                {
                    if (AActor* TargetActor = TargetActors[i].Get())
                    {
                        FTargetDataTraceActor* CurrentData = new FTargetDataTraceActor();
                        CurrentData->TargetActorArray.Add(TargetActor);
                        ReturnDataHandle.Add(CurrentData);
                    }
                }
            }
        }
        else
        {
            ReturnData->TargetActorArray = TargetActors;
        }
        return ReturnDataHandle;
    };

    bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
    {
        SafeNetSerializeTArray_Default<31>(Ar, TargetActorArray);

        bOutSuccess = true;
        return true;
    };
};