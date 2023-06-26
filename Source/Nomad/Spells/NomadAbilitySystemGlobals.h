// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "NomadAbilitySystemGlobals.generated.h"

/**
 * The NomadAbilitySystemGlobals holds predefined spell stage tags.
*/
UCLASS()
class NOMAD_API UNomadAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
	public:
		UPROPERTY()
		FGameplayTag SpellStageTargetingTag;
		UPROPERTY(config)
		FName SpellStageTargetingName = FName("SpellStage.Targeting");

		UPROPERTY()
		FGameplayTag SpellStageTriggeredTag;
		UPROPERTY(config)
		FName SpellStageTriggeredName = FName("SpellStage.Triggered");

		UPROPERTY()
		FGameplayTag SpellStageCastedTag;
		UPROPERTY(config)
		FName SpellStageCastedName = FName("SpellStage.Casted");

		UPROPERTY()
		FGameplayTag SpellStageFinishedTag;
		UPROPERTY(config)
		FName SpellStageFinishedName = FName("SpellStage.Finished");

		UPROPERTY()
		FGameplayTag SpellStageReadyTag;
		UPROPERTY(config)
		FName SpellStageReadyName = FName("SpellStage.Ready");

		UPROPERTY()
		FGameplayTag SpellStageCanceledTag;
		UPROPERTY(config)
		FName SpellStageCanceledName = FName("SpellStage.Canceled");

		void InitGlobalTags() override;

		static UNomadAbilitySystemGlobals& NomadGet() { return dynamic_cast<UNomadAbilitySystemGlobals&>(Get()); }
};
