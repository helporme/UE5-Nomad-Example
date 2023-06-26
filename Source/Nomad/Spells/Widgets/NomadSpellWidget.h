// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "NomadSpellWidgetTypes.h"
#include "NomadSpellWidget.generated.h"

/**
 * The NomadSpellWidget is a helper class that manages all event-handling logic.
*/
UCLASS(Blueprintable)
class NOMAD_API UNomadSpellWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
		UFUNCTION(BlueprintCallable, Category = "Nomad|Spell")
		void BindSpell(FSpellBindingInfo SpellBindingInfo);

		UFUNCTION(BlueprintImplementableEvent, Category = "Nomad|Spell", DisplayName = "On Input Update")
		void K2_OnInputUpdate(FKey NewKey);

		UFUNCTION(BlueprintImplementableEvent, Category = "Nomad|Spell", DisplayName = "On Spell Binded")
		void K2_OnSpellBinded(FSpellBindingInfo SpellBindingInfo);

		UFUNCTION(BlueprintImplementableEvent, Category = "Nomad|Spell", DisplayName = "On Spell Targeting")
		void K2_OnSpellTargeting();

		UFUNCTION(BlueprintImplementableEvent, Category = "Nomad|Spell", DisplayName = "On Spell Triggered")
		void K2_OnSpellTriggered();

		UFUNCTION(BlueprintImplementableEvent, Category = "Nomad|Spell", DisplayName = "On Spell Casted")
		void K2_OnSpellCasted();

		UFUNCTION(BlueprintImplementableEvent, Category = "Nomad|Spell", DisplayName = "On Spell Finished")
		void K2_OnSpellFinished();

		UFUNCTION(BlueprintImplementableEvent, Category = "Nomad|Spell", DisplayName = "On Spell Canceled")
		void K2_OnSpellCanceled();

		UFUNCTION(BlueprintImplementableEvent, Category = "Nomad|Spell", DisplayName = "On Spell Ready")
		void K2_OnSpellReady();
	
	protected:
		UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category="Nomad|Spell")
		FGameplayAbilitySpecHandle SpellHandle;

		UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category="Nomad|Spell")
		TObjectPtr<UNomadSpell> Spell;
		
		UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category="Nomad|Spell")
		TObjectPtr<UNomadSpellSystemComponent> SpellSystem;

		UFUNCTION()
		void UpdateInput();

		void NativeConstruct() override;
		void NativeDestruct() override;
	
	private:
		TArray<FGameplayMessageListenerHandle> MessageListenerHandles;
};
