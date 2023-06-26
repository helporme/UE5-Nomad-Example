// Copyright 2023 Daniel Balatskyi.

#include "SpellReticle.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Nomad/Logging.h"


void ASpellReticle::SetLocation(FVector Location, int Index)
{
    bool IsAlreadyAdded = LocationByIndex.Contains(Index);
    LocationByIndex[Index] = Location;

    if (IsAlreadyAdded)
    {
        OnLocationChanged(Location, Index);
    }
    else
    {
        OnLocationAdded(Location, Index);
    }
}

void ASpellReticle::ClearLocation(int Index)
{
    if (!LocationByIndex.Contains(Index))
    {
        UE_LOG(LogNomad, Warning, TEXT("SpellReticle doesn't contain location with index %i."), Index);
        return;
    }

    FVector Location = LocationByIndex[Index];
    LocationByIndex.Remove(Index);
    OnLocationCleared(Location, Index);
}

void ASpellReticle::ClearAllLocations()
{
    TArray<int> Indices;
    LocationByIndex.GetKeys(Indices); 

    for (int Index : Indices)
    {
        ClearLocation(Index);
    }
}

void ASpellReticle::SetVictims(TArray<FSpellReticleVictimData> VictimsData)
{
    for (auto& VictimData : VictimsData)
    {
        SetVictim(VictimData.Victim.Get(), VictimData.IsFeasible);
    }
}

void ASpellReticle::AddVictim(AActor* Victim, bool IsFeasible)
{
    FString Name = Victim->GetName();

    if (!VictimDataByIndex.Contains(Name))
    {
        VictimDataByIndex.Add(Name, { Victim, IsFeasible });
        OnVictimAdded(Victim, IsFeasible);
    }
}

void ASpellReticle::SetVictim(AActor* Victim, bool IsFeasible)
{
    FString Name = Victim->GetName();

    if (!VictimDataByIndex.Contains(Name))
    {
        ClearAllVictims();
        VictimDataByIndex.Add(Name, { Victim, IsFeasible });
        OnVictimAdded(Victim, IsFeasible);
    }
}

void ASpellReticle::ClearVictim(FString Name)
{
    if (!VictimDataByIndex.Contains(Name))
    {
        UE_LOG(LogNomad, Warning, TEXT("SpellReticle doesn't contain victim %s."), *Name);
        return;
    }

    auto VictimData = VictimDataByIndex[Name];
    VictimDataByIndex.Remove(Name);
    OnVictimCleared(VictimData.Victim.Get(), VictimData.IsFeasible);
}

void ASpellReticle::ClearAllVictims()
{
    TArray<FString> Names;
    VictimDataByIndex.GetKeys(Names);

    for (FString Name : Names)
    {
        ClearVictim(Name);
    }
}

void ASpellReticle::Tick(float DeltaTime)
{
    AActor::Tick(DeltaTime);
    
    if (bFaceTowardSource)
    {
        FaceTowardSource(bFaceOwnerFlat);
    }

    for (auto& Pair : LocationByIndex)
    {
        OnLocationTick(Pair.Value, Pair.Key);
    }

    for (auto& Pair : VictimDataByIndex)
    {
        AActor* Victim = Pair.Value.Victim.Get();
        if (Victim)
        {
            OnVictimTick(Victim, Pair.Value.IsFeasible);
        }
    }
}

void ASpellReticle::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    ClearAllLocations();
    ClearAllVictims();
    Super::EndPlay(EndPlayReason);
}
