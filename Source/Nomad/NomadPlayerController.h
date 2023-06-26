// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "NomadPlayerController.generated.h"


class UNomadInputAction;
class UInputMappingContext;
class UAbilitySystemComponent;
struct FInputActionInstance;

namespace NomadInput
{
	const int MaxMappableOrderedActions = 8;
};

/**
 * The NomadPlayerController directly maps the player's input to the spell system of the character.
 * Lately it may be changed because this approach cannot send data to a spell.
*/
UCLASS()
class NOMAD_API ANomadPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ANomadPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input Actions")
	UInputMappingContext* DefaultMappingContext;

	/**
	 * Input actions that will be mapped to the character's spell system by a unique ID.
	 * The unique ID of an action can be obtained by the methods below.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input Actions")
	TArray<UNomadInputAction*> InputActions; 

public:
	UFUNCTION(BlueprintCallable, Category = Input, DisplayName = "Get ID by Spell Tag")
	int GetIDBySpellTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = Input, DisplayName = "Get ID by Order")
	int GetIDByOrder(int Order);

	UFUNCTION(BlueprintCallable, Category = Input, DisplayName="Get Input Action ID")
	int GetInputActionID(const UNomadInputAction* Action)const;

	UFUNCTION(BlueprintCallable, Category = Input, DisplayName="Find Input Action by ID")
	UNomadInputAction* FindInputActionByID(int ID) const;

protected:
	void SetupInputComponent() override;
	void BeginPlay() override;

	void RegisterInputAction(const UNomadInputAction* Action);
	void BroadcastInputAction(const FInputActionInstance& ActionData);

protected:
	UAbilitySystemComponent* AbilitySystem;

	TMap<FGameplayTag, int> SpellTagIDs;
	TMap<int, int> OrderedSpellIDs;
	TMap<int, UNomadInputAction*> ActionByID;
};
