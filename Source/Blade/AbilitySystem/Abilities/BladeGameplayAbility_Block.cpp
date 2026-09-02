// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeGameplayAbility_Block.h"

#include "AbilitySystemComponent.h"
#include "BladeGameplayTags.h"

UBladeGameplayAbility_Block::UBladeGameplayAbility_Block()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(BladeGameplayTags::Ability_Block);
	SetAssetTags(Tags);
	
	ActivationOwnedTags.AddTag(BladeGameplayTags::State_Blocking);
	
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_Attacking_Committed);
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_Evading);
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_HitReacting);
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_Dead);
	
	CancelAbilitiesWithTag.AddTag(BladeGameplayTags::Ability_Sprint);
	CancelAbilitiesWithTag.AddTag(BladeGameplayTags::Ability_Attack);
}

void UBladeGameplayAbility_Block::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (ensureMsgf(SlowEffect, TEXT("SlowEffect not set on %s"), *GetName()))
	{
		const FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(SlowEffect);
		SlowEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, Spec);
	}
}

void UBladeGameplayAbility_Block::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (SlowEffectHandle.IsValid())
	{
		if (UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo())
		{
			OwnerASC->RemoveActiveGameplayEffect(SlowEffectHandle);
		}
		
		SlowEffectHandle = FActiveGameplayEffectHandle();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
