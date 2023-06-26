// Copyright 2023 Daniel Balatskyi.

#include "NomadSpellWidget.h"
#include "Nomad/Spells/NomadAbilitySystemGlobals.h"
#include "Nomad/NomadPlayerController.h"
#include "Nomad/Input/NomadInputAction.h"
#include "Nomad/Logging.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"


void UNomadSpellWidget::NativeConstruct() 
{
    Super::NativeConstruct();

    auto MakeListener = [this](void(UNomadSpellWidget:: *Function)()) -> TFunction<void(FGameplayTag, const FSpellMessage& Message)> 
    {
        return [this, Function](FGameplayTag Tag, const FSpellMessage& Message) { 
            if (SpellHandle.IsValid() && SpellHandle == Message.SpellHandle)
            {
                (this->*Function)();
            }
        };
    };

    UNomadAbilitySystemGlobals& SystemGlobals = UNomadAbilitySystemGlobals::NomadGet();

    // Register event listeners
    UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
    FGameplayMessageListenerHandle Handles[] = {
        MessageSubsystem.RegisterListener(SystemGlobals.SpellStageTargetingTag, MakeListener(&UNomadSpellWidget::K2_OnSpellTargeting)),
        MessageSubsystem.RegisterListener(SystemGlobals.SpellStageTriggeredTag, MakeListener(&UNomadSpellWidget::K2_OnSpellTriggered)),
        MessageSubsystem.RegisterListener(SystemGlobals.SpellStageCastedTag, MakeListener(&UNomadSpellWidget::K2_OnSpellCasted)),
        MessageSubsystem.RegisterListener(SystemGlobals.SpellStageFinishedTag, MakeListener(&UNomadSpellWidget::K2_OnSpellFinished)),
        MessageSubsystem.RegisterListener(SystemGlobals.SpellStageCanceledTag, MakeListener(&UNomadSpellWidget::K2_OnSpellCanceled)),
        MessageSubsystem.RegisterListener(SystemGlobals.SpellStageReadyTag, MakeListener(&UNomadSpellWidget::K2_OnSpellReady)),
    };

    MessageListenerHandles.Append(Handles, UE_ARRAY_COUNT(Handles));

    ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
    auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

    if (InputSubsystem)
    {
        InputSubsystem->ControlMappingsRebuiltDelegate.AddDynamic(this, &UNomadSpellWidget::UpdateInput);
        UpdateInput();
    }
}

void UNomadSpellWidget::NativeDestruct()
{
    Super::NativeDestruct();

    UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());

    for (FGameplayMessageListenerHandle Handle : MessageListenerHandles)
    {
        MessageSubsystem.UnregisterListener(Handle);
    }
}

void UNomadSpellWidget::BindSpell(FSpellBindingInfo SpellBindingInfo)
{
    SpellHandle = SpellBindingInfo.SpellHandle;
    Spell = SpellBindingInfo.Spell;
    SpellSystem = SpellBindingInfo.SpellSystem;
    

    K2_OnSpellBinded(SpellBindingInfo);
}

void UNomadSpellWidget::UpdateInput()
{
    ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
    if (!LocalPlayer) return;

    const auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
    if (!InputSubsystem) return;

    const ANomadPlayerController* PlayerController = Cast<ANomadPlayerController>(GetOwningPlayer());
    if (!PlayerController) return;

	const FGameplayAbilitySpec* SpellSpec = SpellSystem->FindAbilitySpecFromHandle(SpellHandle);
    const UNomadInputAction* Action = PlayerController->FindInputActionByID(SpellSpec->InputID);

    if (!ensure(Action))
    {
        UE_LOG(LogNomad, Warning, TEXT("Unable to find InputAction by ID (%i)."), SpellSpec->InputID);
    }

    TArray<FKey> Keys = InputSubsystem->QueryKeysMappedToAction(Action);

    if (Keys.Num() > 0)
    {
        K2_OnInputUpdate(Keys[0]);
    }
}