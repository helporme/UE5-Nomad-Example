// Copyright 2023 Daniel Balatskyi.

#include "NomadPlayerState.h"
#include "Spells/NomadSpellSystemComponent.h"
#include "Nomad/Logging.h"


ANomadPlayerState::ANomadPlayerState()
{
	SpellSystem = CreateDefaultSubobject<UNomadSpellSystemComponent>(TEXT("AbilitySystemComponent"));
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

UAbilitySystemComponent* ANomadPlayerState::GetAbilitySystemComponent() const 
{
	return SpellSystem;
}

void ANomadPlayerState::BeginPlay()
{
	// Register SpellSystem tick functions, otherwise tick tasks don't tick.
	SpellSystem->RegisterAllComponentTickFunctions(true);

	APawn* Pawn = GetPawn();

	if (Pawn)
	{
		SpellSystem->SetAvatarActor(Pawn);
	}
	else
	{
		UE_LOG(LogNomad, Error, TEXT("APlayerState->GetPawn() failed."))
	}
}