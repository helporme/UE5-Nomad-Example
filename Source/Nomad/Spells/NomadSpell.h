// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "NomadSpell.generated.h"

struct FGameplayTag;
class UNomadSpellSystemComponent;
class UInputMappingContext;


UENUM()
enum class ESpellStage
{
	Targeting,
	Triggered,
	Canceled,
	Casted,
	Finished,
	Ready
};


/**
 * SpellMessage is the data that is sent to UI.
*/
USTRUCT(BlueprintType)
struct FSpellMessage 
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Nomad|Spells")
	FGameplayAbilitySpecHandle SpellHandle;
};

/**
 * The SpelLCastContext is used to pass data between spell stages.
*/
struct FSpellCastContext
{
	FSpellCastContext(
		const FGameplayAbilitySpecHandle SpellHandle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayAbilityTargetDataHandle TargetData)
		: SpellHandle(SpellHandle)
		, ActorInfo(ActorInfo)
		, ActivationInfo(ActivationInfo)
		, TargetData(TargetData)
		{ };

	
	FSpellCastContext(
		const FGameplayAbilitySpecHandle SpellHandle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo)
		: FSpellCastContext(SpellHandle, ActorInfo, ActivationInfo, FGameplayAbilityTargetDataHandle())
		{ };

	FSpellCastContext()
		: FSpellCastContext(FGameplayAbilitySpecHandle(), nullptr, FGameplayAbilityActivationInfo())
		{ };

	const FGameplayAbilitySpecHandle SpellHandle; 
	const FGameplayAbilityActorInfo* ActorInfo; 
	const FGameplayAbilityActivationInfo ActivationInfo;
	const FGameplayAbilityTargetDataHandle TargetData;
};


/**
 * The NomadSpell is an extended GameplayAbility that implements CastTime, multiple cast stages, and
 * additional UI events with the GameplayMessages plugin. 
*/
UCLASS(Blueprintable)
class NOMAD_API UNomadSpell : public UGameplayAbility
{
	GENERATED_BODY()

	public:
		UNomadSpell(const FObjectInitializer& ObjectInitializer);

		UPROPERTY(EditDefaultsOnly, Category = "Cooldowns")
		TSubclassOf<class UGameplayEffect> CastTimeGameplayEffectClass;

		UPROPERTY(EditDefaultsOnly, Category = "Triggers")
		bool bActivateOnGranted;

		UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputMappingContext* SpellTargetingInputContext;

	public:
		/* Stage */

		UFUNCTION(BlueprintCallable, Category = "Nomad|Spell")
		ESpellStage GetCurrentStage() { return CurrentStage; }


		/* Cooldown */

		UFUNCTION(BlueprintCallable, Category = "Nomad|Spell")
		float GetCooldownDurationInSeconds(UNomadSpellSystemComponent* SpellSystem) const;

		UFUNCTION(BlueprintCallable, Category = "Nomad|Spell")
		float GetCooldownRemainingInSeconds(UNomadSpellSystemComponent* SpellSystem) const;


		/* CastTime */

		UFUNCTION(BlueprintCallable, Category = "Nomad|Spell")
		float GetCastTimeDuration(UNomadSpellSystemComponent* SpellSystem) const;

		UFUNCTION(BlueprintCallable, Category = "Nomad|Spell")
		float GetCastTimeRemaining(UNomadSpellSystemComponent* SpellSystem) const;
	
	protected:
		/* BeginTargeting */

		/**
		 * Called when a spell enters the casting stage. When finished, must call CommitTarget or CancelTarget.
		*/
		virtual void BeginTargeting(const FSpellCastContext& Context, const FGameplayEventData* TriggetEventData);

		/**
		 * Called when a spell enters the casting stage. When finished, must call CommitTarget or CancelTarget.
		*/
		UFUNCTION(BlueprintImplementableEvent, Category = "Nomad|Spell", DisplayName = "BeginTargeting")
		void K2_BeginTargeting();


		/* CommitTarget */

		/**
		 * Begins the PreCast stage.
		*/
		virtual void CommitTarget(const FSpellCastContext& Context, const FGameplayEventData* TriggetEventData);

		UFUNCTION(BlueprintCallable, Category="Nomad|Spell", DisplayName = "CommitTarget")
		void K2_CommitTarget(FGameplayAbilityTargetDataHandle TargetData);


		/* CancelTarget */

		void CancelTarget(const FSpellCastContext& Context);

		UFUNCTION(BlueprintCallable, Category="Nomad|Spell", DisplayName = "CancelTarget")
		void K2_CancelTarget();


		/* PreCast*/

		/** 
		 * Called when a spell enters the PreCast stage. When finished, must call CommitPreCast or EndSpell.
		*/
		virtual void PreCastSpell(const FSpellCastContext& Context, const FGameplayEventData* TriggerEventData);

		/** 
		 * Called when a spell enters the PreCast stage. When finished, must call CommitPreCast or EndSpell.
		*/
		UFUNCTION(BlueprintImplementableEvent, Category = "Nomad|Spell", DisplayName = "PreCastSpell")
		void K2_PreCastSpell();

		/** 
		 * Called when a spell enters the PreCast stage. When finished, must call CommitPreCast or EndSpell.
		*/
		UFUNCTION(BlueprintImplementableEvent, Category = "Nomad|Spell", DisplayName = "PreCastSpellFromEvent")
		void K2_PreCastSpellFromEvent(const FGameplayEventData& TriggerEventData);


		/* CommitPreCast */

		/**
		 * Apply a cast time effect and at the end of the effect begins the Cast stage.
		*/
		virtual bool CommitPreCast(const FSpellCastContext& Context, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr);


		/**
		 * Apply a cast time effect and at the end of the effect begins the Cast stage.
		*/
		UFUNCTION(BlueprintCallable, Category="Nomad|Spell", DisplayName = "CommitPreCast")
		bool K2_CommitPreCast();


		/* Cast */

		/** 
		 * Called after CastTime ended. Use it instead of ActivateAbility. When finished, must call CommitCast and EndSpell.
		*/
		virtual void CastSpell(const FSpellCastContext& Context, const FGameplayEventData* TriggerEventData);

		/** 
		 * Called after CastTime ended. Use it instead of ActivateAbility. When finished, must call CommitCast and EndSpell.
		*/
		UFUNCTION(BlueprintImplementableEvent, Category = "Nomad|Spell", DisplayName = "CastSpell")
		void K2_CastSpell();

		/** 
		 * Called after CastTime ended. Use it instead of ActivateAbility. When finished, must call CommitCast and EndSpell.
		*/
		UFUNCTION(BlueprintImplementableEvent, Category = "Nomad|Spell", DisplayName = "CastSpellFromEvent")
		void K2_CastSpellFromEvent(const FGameplayEventData& TriggerEventData);


		/* CommitCast */

		/**
		 * Begins Finished stage and commits ability.
		*/
		virtual bool CommitCast(const FSpellCastContext& Context, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr);

		UFUNCTION(BlueprintCallable, Category="Nomad|Spell", DisplayName = "CommitCast")
		bool K2_CommitCast();


		/* EndSpell */

		virtual void EndSpell(const FSpellCastContext& Context, bool bReplicateEndAbility, bool bWasCancelled);

		UFUNCTION(BlueprintCallable, Category="Nomad|Spell", DisplayName = "EndSpell")
		void K2_EndSpell();


		/* Cooldown */

        UFUNCTION()
		void OnCooldownEffectRemoved(const FGameplayAbilitySpecHandle& Handle, const FGameplayEffectRemovalInfo& EffectRemovalInfo);

		FActiveGameplayEffectHandle FindCooldownEffect(const FGameplayAbilityActorInfo* ActorInfo) const;


		/* CastTime */

        UFUNCTION()
		void OnCastTimeEffectRemoved(const FGameplayAbilitySpecHandle& Handle, const FGameplayEffectRemovalInfo& EffectRemovalInfo);

		void ApplyCastTime(const FSpellCastContext& Context);

		FActiveGameplayEffectHandle FindCastTimeEffect(const FGameplayAbilityActorInfo* ActorInfo) const;

		UGameplayEffect* GetCastTimeEffect() const;
		
		const FGameplayTagContainer* GetCastTimeEffectTags() const;


		/* GameplayAbility methods */
		
		virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec & Spec) override;

		virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	protected:
		UPROPERTY(BlueprintReadOnly, Category = Spell)
		FGameplayAbilityTargetDataHandle CurrentTargetData;

		ESpellStage CurrentStage;

		bool bHasBlueprintBeginTargeting;
		bool bHasBlueprintPreCastSpell;
		bool bHasBlueprintPreCastSpellFromEvent;
		bool bHasBlueprintCastSpell;
		bool bHasBlueprintCastSpellFromEvent;
};
