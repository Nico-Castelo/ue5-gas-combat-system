// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "BladeGameplayTags.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBladeAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		OwnerMovementComponent = OwnerCharacter->GetCharacterMovement();
		
		if (const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OwnerCharacter))
		{
			OwnerASC = ASI->GetAbilitySystemComponent();
		}
	}
}

void UBladeAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	bIsBlocking = OwnerASC && OwnerASC->HasMatchingGameplayTag(BladeGameplayTags::State_Blocking);
	bIsLockedOn = OwnerASC && OwnerASC->HasMatchingGameplayTag(BladeGameplayTags::State_LockedOn);
	bIsSprinting = OwnerASC && OwnerASC->HasMatchingGameplayTag(BladeGameplayTags::State_Sprinting);
}

void UBladeAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	
	if (!OwnerMovementComponent) return;
	
	GroundSpeed = OwnerMovementComponent->Velocity.Size2D();
	bIsFalling = OwnerMovementComponent->IsFalling();
	VerticalVelocity = OwnerMovementComponent->Velocity.Z;
	Direction = UKismetAnimationLibrary::CalculateDirection(OwnerMovementComponent->Velocity, OwnerCharacter->GetActorRotation());
}
