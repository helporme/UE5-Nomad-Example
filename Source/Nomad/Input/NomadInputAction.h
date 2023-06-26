// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputTriggers.h" 
#include "NomadInputActionType.h"
#include "GameplayTagContainer.h"
#include "NomadInputAction.generated.h"

/**
 * The NomadInputAction is designed to bind the player's input to the particular NomadSpell.   
*/
UCLASS(BlueprintType)
class NOMAD_API UNomadInputAction : public UInputAction
{
	GENERATED_BODY()
	
	public:
		UPROPERTY(EditAnywhere)
		ENomadInputActionType ActionType;

		/* When to cast a spell? */
		UPROPERTY(EditAnywhere)
		ETriggerEvent TriggerEvent;

		UPROPERTY(EditAnywhere, meta=(EditCondition="ActionType == ENomadInputActionType::CastSpellByOrdinal", ClampMin=0, ClampMax=7)) 
		int SpellOrdinalIndex;

		UPROPERTY(EditAnywhere, meta=(EditCondition="ActionType == ENomadInputActionType::CastSpellByTag"))
		FGameplayTag SpellTag;
};
