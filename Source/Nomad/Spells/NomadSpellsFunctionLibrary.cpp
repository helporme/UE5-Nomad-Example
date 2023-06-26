// Copyright 2023 Daniel Balatskyi.

#include "NomadSpellsFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"


FGameplayTargetDataFilterHandle UNomadSpellsFunctionLibrary::MakeComponentFilterHandle(FComponentTargetFilter Filter, AActor* FilterActor)
{
	FGameplayTargetDataFilterHandle FilterHandle;
	FComponentTargetFilter* NewFilter = new FComponentTargetFilter(Filter);

	NewFilter->InitializeFilterContext(FilterActor);
	FilterHandle.Filter = TSharedPtr<FGameplayTargetDataFilter>(NewFilter);
	return FilterHandle;
}

float UNomadSpellsFunctionLibrary::GetFirstNotifiyTriggerTimeByName(UAnimSequenceBase* AnimSequence, FName NotifyName)
{
	if (AnimSequence)
	{
		for (const FAnimNotifyEvent& NotifyEvent : AnimSequence->Notifies)
		{
			if (NotifyEvent.Notify->GetNotifyName() == NotifyName.ToString())
			{
				return NotifyEvent.GetTriggerTime();
				break;
			}
		}
	}

	return -1;
}

FGameplayTagContainer UNomadSpellsFunctionLibrary::GetAbilityTags(TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (AbilityClass)
	{
		return AbilityClass.GetDefaultObject()->AbilityTags;
	}
	return FGameplayTagContainer();
}