// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "NomadInputActionType.generated.h"


UENUM(BlueprintType)
enum class ENomadInputActionType : uint8
{
    /* Casts spell by an ordinal index, traditionally they may be bound to the QWERTY keys. */
    CastSpellByOrdinal UMETA(DisplayName="Cast Spell by Ordinal Index"), 

    /* Casts spell by a gameplay tag. */
    CastSpellByTag UMETA(DisplayName="Cast Spell by Tag"),
};