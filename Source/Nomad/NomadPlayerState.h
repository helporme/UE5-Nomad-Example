// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "NomadPlayerState.generated.h"


class UNomadSpellSystemComponent;


/**
 * The NomadPlayerState contains the AbilitySystemComponent of the character, 
 * it prevents the ability system state from being lost when the player's character dies. 
 * 
 * Note that PlayerState does not register tick functions of components, they must be registered manually.
 * This is especially important for the AbilitySystemComponent because it ticks ability tasks. See ANomadPlayerState::BeginPlay.
*/
UCLASS()
class NOMAD_API ANomadPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	public:
		ANomadPlayerState();

		UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	protected:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
		UNomadSpellSystemComponent* SpellSystem;

		void BeginPlay() override;
};
