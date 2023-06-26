// Copyright 2023 Daniel Balatskyi.

#include "NomadTask.h"


void UNomadTask::Activate()
{
    Super::Activate();
    K2_OnActivate();
}

void UNomadTask::Finish() 
{
    if (ShouldBroadcastAbilityTaskDelegates())
    {
        OnFinished.Broadcast();
    }
    EndTask();
}

void UNomadTask::OnDestroy(bool AbilityIsEnding)
{
    K2_OnDestroy();
    Super::OnDestroy(AbilityIsEnding);
}