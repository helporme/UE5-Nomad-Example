// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "NomadEngineSubsystem.generated.h"


/**
 * The NomadEngineSubsystem needed to call UAbilitySystemGlobals::InitGlobalData to use TargetData.  
*/
UCLASS()
class NOMAD_API UNomadEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
	
	public:
		virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
};
