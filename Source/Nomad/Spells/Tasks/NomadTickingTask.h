// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "NomadTask.h"
#include "NomadTickingTask.generated.h"

UCLASS(Blueprintable)
class NOMAD_API UNomadTickingTask : public UNomadTask 
{
	GENERATED_BODY()

	UNomadTickingTask(const FObjectInitializer& ObjectInitializer);

	public:
		void Activate() override;

		void TickTask(float DeltaTime) override;

		void OnDestroy(bool AbilityIsEnding) override;
        UFUNCTION(BlueprintCallable, Category="Nomad|Tasks", meta=(DeterminesOutputType="Class", HidePin = "ThisAbility", DefaultToSelf = "ThisAbility", ReturnDisplayName="Async Task"))
        static UNomadTickingTask* NewNomadTickingTask(UGameplayAbility* ThisAbility, TSubclassOf<UNomadTickingTask> Class, FName Name, float Duration = -1)
        {
			UNomadTickingTask* MyTask = NewNomadTask<UNomadTickingTask>(ThisAbility, Class, Name);
			MyTask->Duration = Duration;
            return MyTask;
        };

    protected:
        UFUNCTION(BlueprintImplementableEvent, Category = "Task|Events", DisplayName = "Tick")
        void K2_Tick(float DeltaTime);

        UPROPERTY(BlueprintReadWrite)
		float Duration;

        UPROPERTY(BlueprintReadOnly)
		float StartTime;

        UPROPERTY(BlueprintReadOnly)
		float TimeFromActivation;

		bool IsEnded;
};