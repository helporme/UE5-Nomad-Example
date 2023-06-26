// Copyright 2023 Daniel Balatskyi.

#include "NomadTickingTask.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTasksComponent.h"


UNomadTickingTask::UNomadTickingTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    bTickingTask = true;
    IsEnded = false;
}

void UNomadTickingTask::Activate()
{
    StartTime = GetWorld()->GetTimeSeconds();
    Super::Activate();
}

void UNomadTickingTask::TickTask(float DeltaTime)
{
	if (IsEnded) return;

	Super::TickTask(DeltaTime);

    float CurrentTime = GetWorld()->GetTimeSeconds();
    TimeFromActivation = CurrentTime - StartTime;

    if (Duration >= 0 && TimeFromActivation >= Duration)
    {
        Finish();
        return;
    }

    K2_Tick(DeltaTime);
}

void UNomadTickingTask::OnDestroy(bool AbilityIsEnding)
{
	IsEnded = true;
    Super::OnDestroy(AbilityIsEnding);
}