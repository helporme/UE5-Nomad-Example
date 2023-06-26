// Copyright 2023 Daniel Balatskyi.

#include "NomadSpell.h"
#include "NomadSpellSystemComponent.h"
#include "NomadAbilitySystemGlobals.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "Tasks/WaitEffectRemovedTask.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/Engine.h"
#include "UObject/Package.h" 


UNomadSpell::UNomadSpell(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	auto ImplementedInBlueprint = [](const UFunction* Func) -> bool
	{
		return Func && ensure(Func->GetOuter())
			&& Func->GetOuter()->IsA(UBlueprintGeneratedClass::StaticClass());
	};

    {
		static FName FuncName = FName(TEXT("K2_BeginTargeting"));
		UFunction* ShouldRespondFunction = GetClass()->FindFunctionByName(FuncName);
		bHasBlueprintBeginTargeting = ImplementedInBlueprint(ShouldRespondFunction);
    }
    {
		static FName FuncName = FName(TEXT("K2_PreCastSpell"));
		UFunction* ShouldRespondFunction = GetClass()->FindFunctionByName(FuncName);
		bHasBlueprintPreCastSpell = ImplementedInBlueprint(ShouldRespondFunction);
    }
    {
		static FName FuncName = FName(TEXT("K2_PreCastSpellFromEvent"));
		UFunction* ShouldRespondFunction = GetClass()->FindFunctionByName(FuncName);
		bHasBlueprintPreCastSpellFromEvent = ImplementedInBlueprint(ShouldRespondFunction);
    }
    {
		static FName FuncName = FName(TEXT("K2_CastSpell"));
		UFunction* ShouldRespondFunction = GetClass()->FindFunctionByName(FuncName);
		bHasBlueprintCastSpell = ImplementedInBlueprint(ShouldRespondFunction);
    }
    {
		static FName FuncName = FName(TEXT("K2_CastSpellFromEvent"));
		UFunction* ShouldRespondFunction = GetClass()->FindFunctionByName(FuncName);
		bHasBlueprintCastSpellFromEvent = ImplementedInBlueprint(ShouldRespondFunction);
    }

    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}


void UNomadSpell::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnAvatarSet(ActorInfo, Spec);
    if (bActivateOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}


float UNomadSpell::GetCooldownDurationInSeconds(UNomadSpellSystemComponent* SpellSystem) const 
{
    if (!SpellSystem) return -1;

    const FGameplayTagContainer* CooldownTags = GetCooldownTags();
    if (!CooldownTags || CooldownTags->Num() == 0) return -1;

    TArray<float> Durations = SpellSystem->GetActiveEffectsDuration(FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(*CooldownTags));

    if (Durations.Num() > 0)
    {
        Durations.Sort();
        return Durations[Durations.Num() - 1];
    }

    return -1;
}

float UNomadSpell::GetCooldownRemainingInSeconds(UNomadSpellSystemComponent* SpellSystem) const 
{
    if (!SpellSystem) return -1;

    const FGameplayTagContainer* CooldownTags = GetCooldownTags();
    if (!CooldownTags || CooldownTags->Num() == 0) return -1;

    TArray<float> Remaining = SpellSystem->GetActiveEffectsTimeRemaining(FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(*CooldownTags));

    if (Remaining.Num() > 0)
    {
        Remaining.Sort();
        return Remaining[Remaining.Num() - 1];
    }

    return 1;
}

float UNomadSpell::GetCastTimeDuration(UNomadSpellSystemComponent* SpellSystem) const 
{
    if (!SpellSystem) return -1;

    const FGameplayTagContainer* CastTimeTags = GetCastTimeEffectTags();
    if (!CastTimeTags || CastTimeTags->Num() == 0) return -1;

    TArray<float> Durations = SpellSystem->GetActiveEffectsDuration(FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(*CastTimeTags));

    if (Durations.Num() > 0)
    {
        Durations.Sort();
        return Durations[Durations.Num() - 1];
    }

    return -1;
}

float UNomadSpell::GetCastTimeRemaining(UNomadSpellSystemComponent* SpellSystem) const
{
    if (!SpellSystem) return -1;

    const FGameplayTagContainer* CastTimeTags = GetCastTimeEffectTags();
    if (!CastTimeTags || CastTimeTags->Num() == 0) return -1;

    TArray<float> Remaining = SpellSystem->GetActiveEffectsTimeRemaining(FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(*CastTimeTags));

    if (Remaining.Num() > 0)
    {
        Remaining.Sort();
        return Remaining[Remaining.Num() - 1];
    }

    return -1;
}


void UNomadSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) 
{
    if (bHasBlueprintActivate || bHasBlueprintActivateFromEvent)
    {
        Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
        return;
    }

    const FSpellCastContext CastContext(Handle, ActorInfo, ActivationInfo);

    BeginTargeting(CastContext, TriggerEventData);
}


void UNomadSpell::BeginTargeting(const FSpellCastContext& Context, const FGameplayEventData* TriggetEventData)
{
    if (Context.ActorInfo->IsLocallyControlledPlayer())
    {
        APlayerController* PC = Context.ActorInfo->PlayerController.Get();

        if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            InputSubsystem->AddMappingContext(SpellTargetingInputContext, 9);
        }
    }

    CurrentStage = ESpellStage::Targeting;

    UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
    FGameplayTag Tag = UNomadAbilitySystemGlobals::NomadGet().SpellStageTargetingTag;
    MessageSystem.BroadcastMessage(Tag, FSpellMessage { Context.SpellHandle });

    if (bHasBlueprintBeginTargeting)
    {
        K2_BeginTargeting();
    }
    else
    {
        CommitTarget(Context, TriggetEventData);
    }
}

void UNomadSpell::K2_CommitTarget(FGameplayAbilityTargetDataHandle TargetData)
{
    CommitTarget(FSpellCastContext(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, TargetData), &CurrentEventData);
}

void UNomadSpell::CommitTarget(const FSpellCastContext& Context, const FGameplayEventData* TriggerEventData)
{
    if (Context.ActorInfo->IsLocallyControlledPlayer())
    {
        APlayerController* PC = Context.ActorInfo->PlayerController.Get();

        if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            InputSubsystem->RemoveMappingContext(SpellTargetingInputContext);
        }
    }

    CurrentTargetData = Context.TargetData;
    PreCastSpell(Context, TriggerEventData);

    if (!CastTimeGameplayEffectClass)
    {
        CastSpell(Context, TriggerEventData);
    }
}


void UNomadSpell::K2_CancelTarget()
{
    CancelTarget(FSpellCastContext(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo));
}

void UNomadSpell::CancelTarget(const FSpellCastContext& Context)
{
    if (Context.ActorInfo->IsLocallyControlledPlayer())
    {
        APlayerController* PC = Context.ActorInfo->PlayerController.Get();

        if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            InputSubsystem->RemoveMappingContext(SpellTargetingInputContext);
        }
    }

    EndSpell(Context, false, true);
}


void UNomadSpell::PreCastSpell(const FSpellCastContext& Context, const FGameplayEventData* TriggerEventData)
{
    if (TriggerEventData && bHasBlueprintPreCastSpellFromEvent)
    {
        K2_PreCastSpellFromEvent(*TriggerEventData);
    }
    else if (bHasBlueprintPreCastSpell)
    {
        K2_PreCastSpell();
    }
    else
    {
        // Default implementation
        CommitPreCast(Context);
    }
}

bool UNomadSpell::K2_CommitPreCast()
{
	check(CurrentActorInfo);
    return CommitPreCast(FSpellCastContext(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, CurrentTargetData));
}

bool UNomadSpell::CommitPreCast(const FSpellCastContext& Context, OUT FGameplayTagContainer* OptionalRelevantTags)
{
    bool Checked = CommitCheck(Context.SpellHandle, Context.ActorInfo, Context.ActivationInfo, OptionalRelevantTags);
    if (!Checked) return false;
    
    CurrentStage = ESpellStage::Triggered;

    ApplyCastTime(Context);

    if (CastTimeGameplayEffectClass)
    {
        FActiveGameplayEffectHandle CastTimeEffectHandle = FindCastTimeEffect(Context.ActorInfo);
        UAbilitySystemComponent* ASC = Context.ActorInfo->AbilitySystemComponent.Get();
        auto* Task = UWaitEffectRemovedTask::WaitForGameplayEffectRemoved(this, Context.SpellHandle, CastTimeEffectHandle);
        Task->OnRemoved.AddDynamic(this, &UNomadSpell::OnCastTimeEffectRemoved);
        Task->ReadyForActivation();
    }

    UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
    FGameplayTag Tag = UNomadAbilitySystemGlobals::NomadGet().SpellStageTriggeredTag;
    MessageSystem.BroadcastMessage(Tag, FSpellMessage { Context.SpellHandle });

    return true;
}

void UNomadSpell::ApplyCastTime(const FSpellCastContext& Context)
{
    UGameplayEffect* CastTimeGE = GetCastTimeEffect();

    if (CastTimeGE)
    {
        ApplyGameplayEffectToOwner(
            Context.SpellHandle, Context.ActorInfo, Context.ActivationInfo, 
            CastTimeGE, GetAbilityLevel(Context.SpellHandle, Context.ActorInfo));
    }
}

void UNomadSpell::OnCastTimeEffectRemoved(const FGameplayAbilitySpecHandle& Handle, const FGameplayEffectRemovalInfo& EffectRemovalInfo)
{
    CastSpell(FSpellCastContext(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, CurrentTargetData), &CurrentEventData);
}


void UNomadSpell::CastSpell(const FSpellCastContext& Context, const FGameplayEventData* TriggerEventData)
{
    if (TriggerEventData && bHasBlueprintCastSpellFromEvent)
    {
        K2_CastSpellFromEvent(*TriggerEventData);
    }
    else if (bHasBlueprintCastSpell)
    {
        K2_CastSpell();
    }
    else
    {
        // Default implementation
        CommitCast(Context);
        EndSpell(Context, true, false);
    }
}

bool UNomadSpell::K2_CommitCast()
{
	check(CurrentActorInfo);
    return CommitCast(FSpellCastContext(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, CurrentTargetData));
}

bool UNomadSpell::CommitCast(const FSpellCastContext& Context, OUT FGameplayTagContainer* OptionalRelevantTags)
{
    bool Casted = CommitAbility(Context.SpellHandle, Context.ActorInfo, Context.ActivationInfo, OptionalRelevantTags);

    if (Casted)
    {
        CurrentStage = ESpellStage::Casted;

        UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
        FGameplayTag Tag = UNomadAbilitySystemGlobals::NomadGet().SpellStageCastedTag;
        MessageSystem.BroadcastMessage(Tag, FSpellMessage { Context.SpellHandle });
    }

    return Casted;
}

void UNomadSpell::K2_EndSpell()
{
	check(CurrentActorInfo);
    EndSpell(FSpellCastContext(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, CurrentTargetData), true, false);
}

void UNomadSpell::EndSpell(const FSpellCastContext& Context, bool bReplicateEndAbility, bool bWasCancelled) 
{ 
    EndAbility(Context.SpellHandle, Context.ActorInfo, Context.ActivationInfo, bReplicateEndAbility, bWasCancelled); 
    CurrentTargetData = FGameplayAbilityTargetDataHandle();

    FGameplayTag StageTag;

    if (!bWasCancelled)
    {
        FActiveGameplayEffectHandle CooldownEffectHandle = FindCooldownEffect(Context.ActorInfo);
        UAbilitySystemComponent* ASC = Context.ActorInfo->AbilitySystemComponent.Get();

        auto* Task = UWaitEffectRemovedTask::WaitForGameplayEffectRemoved(this, Context.SpellHandle, CooldownEffectHandle);
        Task->OnRemoved.AddDynamic(this, &UNomadSpell::OnCooldownEffectRemoved);
        Task->ReadyForActivation();

        StageTag = UNomadAbilitySystemGlobals::NomadGet().SpellStageFinishedTag;
    }
    else
    {
        StageTag = UNomadAbilitySystemGlobals::NomadGet().SpellStageCanceledTag;
    }

    UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
    MessageSystem.BroadcastMessage(StageTag, FSpellMessage { Context.SpellHandle });

    if (GetCooldownGameplayEffect() == nullptr)
    {
        OnCooldownEffectRemoved(Context.SpellHandle, FGameplayEffectRemovalInfo());
    }
}

void UNomadSpell::OnCooldownEffectRemoved(const FGameplayAbilitySpecHandle& Handle, const FGameplayEffectRemovalInfo&)
{
    UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
    FGameplayTag Tag = UNomadAbilitySystemGlobals::NomadGet().SpellStageReadyTag;
    MessageSystem.BroadcastMessage(Tag, FSpellMessage { CurrentSpecHandle });
}


FActiveGameplayEffectHandle UNomadSpell::FindCastTimeEffect(const FGameplayAbilityActorInfo* ActorInfo) const
{
	const UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    if (!ASC) return FActiveGameplayEffectHandle();

    const FGameplayTagContainer* CastTimeTags = GetCastTimeEffectTags();
    if (!CastTimeTags || CastTimeTags->Num() == 0) return FActiveGameplayEffectHandle();
    
    const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(*CastTimeTags);
	TArray<FActiveGameplayEffectHandle> EffectHandles = ASC->GetActiveEffects(Query);
    
    if (EffectHandles.Num() == 0) return FActiveGameplayEffectHandle();

    return EffectHandles[0];
}

const FGameplayTagContainer* UNomadSpell::GetCastTimeEffectTags() const 
{
    UGameplayEffect* CastTimeEffect = GetCastTimeEffect();

    return CastTimeEffect ? &CastTimeEffect->InheritableOwnedTagsContainer.CombinedTags : nullptr;
}

UGameplayEffect* UNomadSpell::GetCastTimeEffect() const
{
    if (CastTimeGameplayEffectClass)
    {
        return CastTimeGameplayEffectClass->GetDefaultObject<UGameplayEffect>();
    }
    else 
    {
        return nullptr;
    }
}

FActiveGameplayEffectHandle UNomadSpell::FindCooldownEffect(const FGameplayAbilityActorInfo* ActorInfo) const 
{
	const UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    if (!ASC) return FActiveGameplayEffectHandle();

    const FGameplayTagContainer* CooldownTags = GetCooldownTags();
    if (!CooldownTags || CooldownTags->Num() == 0) return FActiveGameplayEffectHandle();
    
    const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(*CooldownTags);
	TArray<FActiveGameplayEffectHandle> EffectHandles = ASC->GetActiveEffects(Query);
    
    if (EffectHandles.Num() == 0) return FActiveGameplayEffectHandle();

    return EffectHandles[0];
}