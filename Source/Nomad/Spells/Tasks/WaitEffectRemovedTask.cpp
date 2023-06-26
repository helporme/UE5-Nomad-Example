// Copyright 2023 Daniel Balatskyi.

#include "WaitEffectRemovedTask.h"
#include "AbilitySystemComponent.h"


UWaitEffectRemovedTask* UWaitEffectRemovedTask::WaitForGameplayEffectRemoved(UGameplayAbility* OwningAbility, FGameplayAbilitySpecHandle AbilityHandle, FActiveGameplayEffectHandle EffectHandle)
{
	UWaitEffectRemovedTask* MyObj = NewAbilityTask<UWaitEffectRemovedTask>(OwningAbility);
	MyObj->EffectHandle = EffectHandle;
    MyObj->AbilityHandle = AbilityHandle;

	return MyObj;
}

void UWaitEffectRemovedTask::Activate()
{
	FGameplayEffectRemovalInfo EmptyGameplayEffectRemovalInfo;

	if (EffectHandle.IsValid() == false)
	{
		EndTask();
		return;
	}

	UAbilitySystemComponent* EffectOwningAbilitySystemComponent = EffectHandle.GetOwningAbilitySystemComponent();

	if (EffectOwningAbilitySystemComponent)
	{
		FOnActiveGameplayEffectRemoved_Info* DelPtr = EffectOwningAbilitySystemComponent->OnGameplayEffectRemoved_InfoDelegate(EffectHandle);
		if (DelPtr)
		{
			OnGameplayEffectRemovedDelegateHandle = DelPtr->AddUObject(this, &UWaitEffectRemovedTask::OnGameplayEffectRemoved);
			Registered = true;
		}
	}

	if (!Registered)
	{
		// GameplayEffect was already removed, treat this as a warning? Could be cases of immunity or chained gameplay rules that would instant remove something
		OnGameplayEffectRemoved(EmptyGameplayEffectRemovalInfo);
	}
}

void UWaitEffectRemovedTask::OnDestroy(bool AbilityIsEnding)
{
	UAbilitySystemComponent* EffectOwningAbilitySystemComponent = EffectHandle.GetOwningAbilitySystemComponent();
	if (EffectOwningAbilitySystemComponent)
	{
		FOnActiveGameplayEffectRemoved_Info* DelPtr = EffectOwningAbilitySystemComponent->OnGameplayEffectRemoved_InfoDelegate(EffectHandle);
		if (DelPtr)
		{
			DelPtr->Remove(OnGameplayEffectRemovedDelegateHandle);
		}
	}

	Super::OnDestroy(AbilityIsEnding);
}

void UWaitEffectRemovedTask::OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo& InGameplayEffectRemovalInfo)
{
    OnRemoved.Broadcast(AbilityHandle, InGameplayEffectRemovalInfo);
	EndTask();
}



