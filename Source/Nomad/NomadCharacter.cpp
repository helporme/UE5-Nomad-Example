// Copyright 2023 Daniel Balatskyi.

#include "NomadCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "NomadPlayerState.h"


// Sets default values
ANomadCharacter::ANomadCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	CameraArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArmComponent"));
	CameraArmComponent->SetupAttachment(RootComponent);
	CameraArmComponent->SetUsingAbsoluteRotation(true);
	CameraArmComponent->TargetArmLength = 800.f;
	CameraArmComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraArmComponent, USpringArmComponent::SocketName);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}


UAbilitySystemComponent* ANomadCharacter::GetAbilitySystemComponent() const
{
	auto PlayerController = GetController();
	check(Controller);

	auto NomadPlayerState = PlayerController->GetPlayerState<ANomadPlayerState>();
	check(NomadPlayerState);

	return NomadPlayerState->GetAbilitySystemComponent();
}