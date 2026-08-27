// Fill out your copyright notice in the Description page of Project Settings.


#include "BladePlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "BladeGameplayTags.h"
#include "BladeLockOnComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ABladePlayerCharacter::ABladePlayerCharacter()
{
	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	LockOnComponent = CreateDefaultSubobject<UBladeLockOnComponent>(TEXT("LockOnComp"));
}

void ABladePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInput->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ABladePlayerCharacter::Move);
	EnhancedInput->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ABladePlayerCharacter::Look);
	
	EnhancedInput->BindAction(Input_Jump, ETriggerEvent::Started, this, &ABladePlayerCharacter::Jump);
	EnhancedInput->BindAction(Input_Jump, ETriggerEvent::Completed, this, &ABladePlayerCharacter::StopJumping);	
	
	EnhancedInput->BindAction(Input_Attack, ETriggerEvent::Started, this, &ABladePlayerCharacter::Attack);
	
	EnhancedInput->BindAction(Input_Block, ETriggerEvent::Triggered, this, &ABladePlayerCharacter::BlockStart);
	EnhancedInput->BindAction(Input_Block, ETriggerEvent::Completed, this, &ABladePlayerCharacter::BlockEnd);
	
	EnhancedInput->BindAction(Input_Evade, ETriggerEvent::Triggered, this, &ABladePlayerCharacter::Evade);
	
	EnhancedInput->BindAction(Input_Sprint, ETriggerEvent::Triggered, this, &ABladePlayerCharacter::SprintStart);
	EnhancedInput->BindAction(Input_Sprint, ETriggerEvent::Completed, this, &ABladePlayerCharacter::SprintEnd);
	
	EnhancedInput->BindAction(Input_LockOn, ETriggerEvent::Started, this, &ABladePlayerCharacter::LockOn);
}

void ABladePlayerCharacter::Move(const FInputActionValue& InValue)
{
	const FVector2D InputValue = InValue.Get<FVector2D>();
	
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	const FVector ForwardDirection = ControlRot.Vector();
	const FVector RightDirection = ControlRot.RotateVector(FVector::RightVector);

	// Forward / Back
	AddMovementInput(ForwardDirection, InputValue.Y);

	// Right / Left
	AddMovementInput(RightDirection, InputValue.X);
}

void ABladePlayerCharacter::Look(const FInputActionValue& InValue)
{
	const FVector2D InputValue = InValue.Get<FVector2D>();
	
	AddControllerPitchInput(InputValue.Y);
	AddControllerYawInput(InputValue.X);
}

void ABladePlayerCharacter::Attack()
{
	ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(BladeGameplayTags::Ability_Attack));
}

void ABladePlayerCharacter::BlockStart()
{
	ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(BladeGameplayTags::Ability_Block));
}

void ABladePlayerCharacter::BlockEnd()
{
	FGameplayTagContainer BlockTags(BladeGameplayTags::Ability_Block);
	ASC->CancelAbilities(&BlockTags);
}

void ABladePlayerCharacter::Evade()
{
	ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(BladeGameplayTags::Ability_Evade));
}

void ABladePlayerCharacter::SprintStart()
{
	ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(BladeGameplayTags::Ability_Sprint));
}

void ABladePlayerCharacter::SprintEnd()
{
	FGameplayTagContainer SprintTags(BladeGameplayTags::Ability_Sprint);
	ASC->CancelAbilities(&SprintTags);
}

void ABladePlayerCharacter::LockOn()
{
	LockOnComponent->ToggleLockOn();
}
