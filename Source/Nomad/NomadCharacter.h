// Copyright 2023 Daniel Balatskyi.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "NomadCharacter.generated.h"

class UAbilitySystemComponent;

/**
 * The most common third-person character setup.
*/
UCLASS(Blueprintable)
class NOMAD_API ANomadCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ANomadCharacter();

	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	FORCEINLINE class USpringArmComponent* GetcameraArmComponent() const { return CameraArmComponent; }

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Camera)
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Camera)
	USpringArmComponent* CameraArmComponent;
};
