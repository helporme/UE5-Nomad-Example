// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "WaitEffectRemovedTask.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWaitGameplayEffectRemovedDelegate, const FGameplayAbilitySpecHandle&, AbilityHandle, const FGameplayEffectRemovalInfo &, GameplayEffectRemovalInfo);

UCLASS()
class NOMAD_API UWaitEffectRemovedTask : public UAbilityTask
{
    GENERATED_BODY()

    public:
        UPROPERTY(BlueprintAssignable)
        FWaitGameplayEffectRemovedDelegate	OnRemoved;

        virtual void Activate() override;

        UFUNCTION()
        void OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo& InGameplayEffectRemovalInfo);

        UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
        static UWaitEffectRemovedTask* WaitForGameplayEffectRemoved(UGameplayAbility* OwningAbility, FGameplayAbilitySpecHandle AbilityHandle, FActiveGameplayEffectHandle EffectHandle);


protected:

	virtual void OnDestroy(bool AbilityIsEnding) override;

	bool Registered;

    FActiveGameplayEffectHandle EffectHandle;
    FGameplayAbilitySpecHandle AbilityHandle;
	FDelegateHandle OnGameplayEffectRemovedDelegateHandle;
};