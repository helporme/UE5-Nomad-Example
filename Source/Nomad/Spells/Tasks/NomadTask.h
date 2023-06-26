// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "NomadTask.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTransformDelegate);

/*
 * The NomadTask exposes AbilityTask to the blueprint. 
 * It doesn't tick, to use a ticking version of this task you need to implement the NomadTickingTask.
*/
UCLASS(Blueprintable, Abstract)
class NOMAD_API UNomadTask : public UAbilityTask 
{
	GENERATED_BODY()

    public:
        /**
         * Initialize a new NomadTask that can be activated via NomadTask::Activate() and finished via NomadTask::FinishTask().
        */
        template <typename T>
        static T* NewNomadTask(UGameplayAbility* ThisAbility, TSubclassOf<UNomadTask> Class, FName Name = FName())
        {
            check(ThisAbility);

            T* MyTask = NewObject<T>(GetTransientPackage(), Class.Get());
            MyTask->InitTask(*ThisAbility, ThisAbility->GetGameplayTaskDefaultPriority());

            UAbilityTask::DebugRecordAbilityTaskCreatedByAbility(ThisAbility);

            MyTask->InstanceName = Name;
            return MyTask;
        };

        /**
         * Initialize a new NomadTask that can be activated via NomadTask::Activate() and finished via NomadTask::Finish().
        */
        UFUNCTION(BlueprintCallable, Category="Nomad|Tasks", meta=(DeterminesOutputType="Class", HidePin = "ThisAbility", DefaultToSelf = "ThisAbility", ReturnDisplayName="Async Task"))
        static UNomadTask* NewNomadTask(UGameplayAbility* ThisAbility, TSubclassOf<UNomadTask> Class, FName Name)
        {
            return NewNomadTask<UNomadTask>(ThisAbility, Class, Name);
        };

        /**
         * Initialize a new NomadTask that can be activated via NomadTask::Activate() and finished via NomadTask::Finish().
        */
        UFUNCTION(BlueprintCallable, Category="Nomad|Tasks", meta=(DeterminesOutputType="Class", HidePin = "ThisAbility", DefaultToSelf = "ThisAbility", ReturnDisplayName="Async Task"))
        static UNomadTask* RunNomadTask(UGameplayAbility* ThisAbility, TSubclassOf<UNomadTask> Class, FName Name)
        {
            UNomadTask* MyTask = NewNomadTask<UNomadTask>(ThisAbility, Class, Name);
            MyTask->ReadyForActivation();
            return MyTask;
        };

    public:
		UPROPERTY(BlueprintAssignable)
		FTransformDelegate OnFinished;

        UFUNCTION(BlueprintCallable, Category = "Task|Events", DisplayName = "Finish Task")
        void Finish();

        void Activate() override;
		void OnDestroy(bool AbilityIsEnding) override;

    protected:
        UFUNCTION(BlueprintImplementableEvent, Category = "Task|Events", DisplayName = "On Activate")
        void K2_OnActivate();

        UFUNCTION(BlueprintImplementableEvent, Category = "Task|Events", DisplayName = "On Destroy")
        void K2_OnDestroy();

        /* Bindings */

        UFUNCTION(BlueprintCallable, Category = "Task", DisplayName = "Activate")
        void K2_ActivateTask() { ReadyForActivation(); };

        UFUNCTION(BlueprintCallable, Category = "Task", DisplayName = "Get Avatar Actor")
        AActor* K2_GetAvatarActor() { return GetAvatarActor(); };

        UFUNCTION(BlueprintCallable, Category = "Task", DisplayName = "Get Ability")
        UGameplayAbility* K2_GetAbility() { return Ability; };

        UFUNCTION(BlueprintCallable, Category = "Task", DisplayName = "Get Ability System Component")
        UAbilitySystemComponent* K2_GetAbilitySystemComponent() { return AbilitySystemComponent.Get(); };
};