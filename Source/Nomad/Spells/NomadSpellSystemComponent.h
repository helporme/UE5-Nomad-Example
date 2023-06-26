// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "Containers/Array.h"
#include "AbilitySystemComponent.h"
#include "NomadSpellSystemComponent.generated.h"

/*
 * The NomadSpellSystem provides additional events that can be used by UI.
*/
UCLASS()
class NOMAD_API UNomadSpellSystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	UDELEGATE()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellEventDelegate, const FGameplayAbilitySpecHandle&, AbilitySpecHandle);

	public:
		UPROPERTY(BlueprintAssignable, Category="Nomad|Abilities")
		FSpellEventDelegate OnAbilityGiven;

		UPROPERTY(BlueprintAssignable, Category="Nomad|Abilities")
		FSpellEventDelegate OnAbilityRemoved;

	protected:
		void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
		void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;
};
