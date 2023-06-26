// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityWorldReticle.h"
#include "SpellReticle.generated.h"



USTRUCT(Blueprintable)
struct FSpellReticleVictimData
{	
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> Victim;

	UPROPERTY(BlueprintReadOnly)
	bool IsFeasible;
};


UCLASS(Blueprintable)
class NOMAD_API ASpellReticle : public AGameplayAbilityWorldReticle
{
	GENERATED_BODY()
	
	public:
		void SetSourceActor(AActor* Actor) { SourceActor = Actor; };

		/**
		 * Called when a spell targets a specific location. Can be called every tick. 
		*/
		UFUNCTION(BlueprintCallable, Category=Reticle)
		void SetLocation(FVector Location, int Index);

		UFUNCTION(BlueprintCallable, Category=Reticle)
		void ClearLocation(int Index);

		UFUNCTION(BlueprintCallable, Category=Reticle)
		void ClearAllLocations();

		/**
		 * Called when a spell targets multiple actors.
		*/
		UFUNCTION(BlueprintCallable, Category=Reticle)
		void SetVictims(TArray<FSpellReticleVictimData> VictimsData);

		/**
		 * Called when a spell progressively targets actors.
		*/
		UFUNCTION(BlueprintCallable, Category=Reticle)
		void AddVictim(AActor* Victim, bool IsFeasible);

		/**
		 * Called when a spell targets an actor.
		*/
		UFUNCTION(BlueprintCallable, Category=Reticle)
		void SetVictim(AActor* Victim, bool IsFeasible);

		UFUNCTION(BlueprintCallable, Category=Reticle)
		void ClearVictim(FString Name);

		UFUNCTION(BlueprintCallable, Category=Reticle)
		void ClearAllVictims();
	
	public:
		void Tick(float DeltaTime) override;

		void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
		
		UPROPERTY(EditAnywhere, Category = Reticle)
		bool bFaceTowardSource;

	protected:
		/* Location */

		UFUNCTION(BlueprintImplementableEvent, Category = Reticle)
		void OnLocationAdded(FVector Location, int Index);

		UFUNCTION(BlueprintImplementableEvent, Category = Reticle)
		void OnLocationChanged(FVector Location, int Index);

		UFUNCTION(BlueprintImplementableEvent, Category = Reticle)
		void OnLocationTick(FVector Location, int Index);

		UFUNCTION(BlueprintImplementableEvent, Category = Reticle)
		void OnLocationCleared(FVector Location, int Index);


		/* Victim */

		UFUNCTION(BlueprintImplementableEvent, Category = Reticle)
		void OnVictimAdded(AActor* Victim, bool IsFeasible);

		UFUNCTION(BlueprintImplementableEvent, Category = Reticle)
		void OnVictimTick(AActor* Victim, bool IsFeasible);

		UFUNCTION(BlueprintImplementableEvent, Category = Reticle)
		void OnVictimCleared(AActor* Victim, bool IsFeasible);

	protected:
		UPROPERTY(BlueprintReadonly)
		TWeakObjectPtr<AActor> SourceActor;

		UPROPERTY(BlueprintReadonly)
		TMap<int, FVector> LocationByIndex;

		UPROPERTY(BlueprintReadonly)
		TMap<FString, FSpellReticleVictimData> VictimDataByIndex;
};
