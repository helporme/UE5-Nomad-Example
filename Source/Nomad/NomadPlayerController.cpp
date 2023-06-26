// Copyright 2023 Daniel Balatskyi.

#include "NomadPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NomadPlayerState.h"
#include "Input/NomadInputAction.h"
#include "AbilitySystemComponent.h"


ANomadPlayerController::ANomadPlayerController() 
{ 
    bEnableClickEvents = true;
    bShowMouseCursor = true;

    SpellTagIDs = {};
    OrderedSpellIDs = {};
    ActionByID = {};
}

void ANomadPlayerController::BeginPlay() 
{
    Super::BeginPlay();

    ANomadPlayerState* State = GetPlayerState<ANomadPlayerState>();
    AbilitySystem = State->GetAbilitySystemComponent();

    if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
    }
}


/**
 * Registers all actions and binds them to the BroadcastInputActino method.  
*/
void ANomadPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        for (UNomadInputAction* Action : InputActions)
        {
            RegisterInputAction(Action);

            int ID = GetInputActionID(Action);
            if (ID != -1)
            {
                ActionByID.Add(ID, Action);
                EnhancedInput->BindAction(Action, Action->TriggerEvent, this, &ANomadPlayerController::BroadcastInputAction);
            }
        }
    }
}

void ANomadPlayerController::RegisterInputAction(const UNomadInputAction* Action)
{
    switch (Action->ActionType)
    {
        case ENomadInputActionType::CastSpellByTag: 
            SpellTagIDs.Add(Action->SpellTag, NomadInput::MaxMappableOrderedActions + SpellTagIDs.Num());
    }
}

void ANomadPlayerController::BroadcastInputAction(const FInputActionInstance& ActionData)
{
    const UNomadInputAction* SourceAction = Cast<UNomadInputAction>(ActionData.GetSourceAction());
    if (!SourceAction) return;

    AbilitySystem->PressInputID(GetInputActionID(SourceAction));
    AbilitySystem->ReleaseInputID(GetInputActionID(SourceAction));
}

int ANomadPlayerController::GetIDBySpellTag(FGameplayTag Tag)
{
    if (Tag.IsValid() && SpellTagIDs.Contains(Tag))
    {
        return *SpellTagIDs.Find(Tag);
    }

    return -1;
}

int ANomadPlayerController::GetIDByOrder(int Order)
{
    using namespace NomadInput;

    if (Order >= 0 && Order < MaxMappableOrderedActions)
    {
        return Order;
    }

    return -1;
}

int ANomadPlayerController::GetInputActionID(const UNomadInputAction* Action) const
{
    using namespace NomadInput;

    switch (Action->ActionType)
    {
        case ENomadInputActionType::CastSpellByOrdinal: 
            if (Action->SpellOrdinalIndex >= 0 && Action->SpellOrdinalIndex < MaxMappableOrderedActions)
            {
                return Action->SpellOrdinalIndex;
            }
        
        case ENomadInputActionType::CastSpellByTag:
            if (Action->SpellTag.IsValid() && SpellTagIDs.Contains(Action->SpellTag))
            {
                return *SpellTagIDs.Find(Action->SpellTag);
            }
    }

    return -1;
}

UNomadInputAction* ANomadPlayerController::FindInputActionByID(int ID) const
{
    if (ActionByID.Contains(ID))
    {
        return *ActionByID.Find(ID);
    }

    return nullptr;
}