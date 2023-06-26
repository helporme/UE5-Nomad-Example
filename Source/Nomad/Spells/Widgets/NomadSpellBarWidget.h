// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Nomad/Spells/NomadSpellSystemComponent.h"
#include "NomadSpellWidgetTypes.h"
#include "NomadSpellBarWidget.generated.h"

/**
 * The NomadSpellBarWidget is a helper class to simplify blueprint logic.
 * Shows spells that the SpellSystem currently contains.
*/
UCLASS(Blueprintable)
class NOMAD_API UNomadSpellBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
        UPROPERTY(BlueprintReadonly)
        TObjectPtr<UNomadSpellSystemComponent> SpellSystem;

	public:
		UFUNCTION(BlueprintCallable, Category = "Nomad|Spells")
        void BindSpellSystem(UNomadSpellSystemComponent* NewSpellSystem);

		UFUNCTION(BlueprintImplementableEvent, Category = "Nomad|Spells")
		void OnSpellAdded(FSpellBindingInfo SpellBindingInfo);

		UFUNCTION(BlueprintImplementableEvent, Category = "Nomad|Spells")
		void OnSpellRemoved(FSpellBindingInfo SpellBindingInfo);
	
	protected:
		FGameplayTag SpellVisibleTag;
		const FName SpellVisibleTagName = FName("SpellMeta.Visible");

		bool Initialize() override;
};