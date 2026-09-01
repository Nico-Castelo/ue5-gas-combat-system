// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeGameplayAbility_Evade.h"

#include "AbilitySystemComponent.h"
#include "Blade.h"
#include "BladeGameplayTags.h"
#include "KismetAnimationLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBladeGameplayAbility_Evade::UBladeGameplayAbility_Evade()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(BladeGameplayTags::Ability_Evade);
	SetAssetTags(Tags);

	ActivationOwnedTags.AddTag(BladeGameplayTags::State_Evading);
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_Evading);
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_Attacking_Committed);
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_HitReacting);
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_Dead);
	
	CancelAbilitiesWithTag.AddTag(BladeGameplayTags::Ability_Block);
}

void UBladeGameplayAbility_Evade::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	UAnimMontage* MontageToPlay = EvadeMontage;
	if (ASC && ASC->HasMatchingGameplayTag(BladeGameplayTags::State_LockedOn))
	{
		
		const AActor* Avatar = GetAvatarActorFromActorInfo();
		
		const UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(ActorInfo->MovementComponent.Get());
		if (Avatar && CharacterMovementComponent)
		{
			const FVector MoveInput = CharacterMovementComponent->GetPendingInputVector();
			
			float EvadeAngle = 0.0f;
			
			if (!MoveInput.IsNearlyZero())
			{
				EvadeAngle = UKismetAnimationLibrary::CalculateDirection(MoveInput, Avatar->GetActorRotation());
				
				const int32 DirectionIndex = (FMath::RoundToInt(EvadeAngle / 45.0f) + 8) % 8;
				
				if (DirectionalEvadeMontages.IsValidIndex(DirectionIndex) && DirectionalEvadeMontages[DirectionIndex])
				{
					MontageToPlay = DirectionalEvadeMontages[DirectionIndex];
				}
				else
				{
					ensureMsgf(false, TEXT("DirectionalEvadeMontages: Invalid index %d"), DirectionIndex);
				}
			}
		}
	}
	
	UAbilityTask_WaitGameplayEvent* WaitRecover = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, BladeGameplayTags::Event_Montage_Recover, nullptr, true);
	WaitRecover->EventReceived.AddDynamic(this, &UBladeGameplayAbility_Evade::OnRecoveryStarted);
	WaitRecover->ReadyForActivation();

	PlayMontageAndEndOnCompletion(MontageToPlay, Rate, RootMotionScale);

	UE_LOG(LogGame, Verbose, TEXT("Evade activated on %s"), *GetNameSafe(GetAvatarActorFromActorInfo()));
}

void UBladeGameplayAbility_Evade::OnMontageBlendOut()
{
	Super::OnMontageBlendOut();
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UBladeGameplayAbility_Evade::OnRecoveryStarted(FGameplayEventData Payload)
{
	const UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent.Get());
	if (!MovementComponent) return;

	FVector MoveInput = MovementComponent->GetPendingInputVector();
	if (MoveInput.IsNearlyZero())
	{
		MoveInput = MovementComponent->GetLastInputVector();
	}
	if (MoveInput.IsNearlyZero()) return;

	const ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Character) return;
	
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!ensure(AnimInstance)) return;

	UAnimMontage* MontageToStop = GetCurrentMontage();
	if (!MontageToStop)
	{
		MontageToStop = EvadeMontage;
	}
	
	if (!MontageToStop) return;

	AnimInstance->Montage_Stop(RecoveryCancelBlendOut, MontageToStop);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
