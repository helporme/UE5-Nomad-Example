// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetDataFilter.h"
#include "Targeting/ComponentTargetFilter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Animation/AnimSequenceBase.h"
#include "NomadSpellsFunctionLibrary.generated.h"


/**
 * The NomadSpellsFunctionLibrary exposes utility functionality to blueprints.
*/
UCLASS(meta=(ScriptName="SpellsSystemLibrary"))
class NOMAD_API UNomadSpellsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintPure, Category = "Filter")
	static FGameplayTargetDataFilterHandle MakeComponentFilterHandle(FComponentTargetFilter Filter, AActor* FilterActor);

	UFUNCTION(BlueprintPure, Category = "Animation")
	static float GetFirstNotifiyTriggerTimeByName(UAnimSequenceBase* AnimSequence, FName NotifyName);

	UFUNCTION(BlueprintPure, Category = "Ability")
	static FGameplayTagContainer GetAbilityTags(TSubclassOf<UGameplayAbility> AbilityClass);
};
