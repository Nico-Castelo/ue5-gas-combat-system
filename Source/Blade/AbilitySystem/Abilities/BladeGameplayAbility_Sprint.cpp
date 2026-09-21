// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeGameplayAbility_Sprint.h"

#include "AbilitySystemComponent.h"
#include "BladeGameplayTags.h"

UBladeGameplayAbility_Sprint::UBladeGameplayAbility_Sprint()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(BladeGameplayTags::Ability_Sprint);
	SetAssetTags(Tags);
	
	ActivationOwnedTags.AddTag(BladeGameplayTags::State_Sprinting);
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_Blocking);
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_Dead);
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_PostureBroken);
}

void UBladeGameplayAbility_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (ensureMsgf(SprintEffect, TEXT("SprintEffect not set on %s"), *GetName()))
	{
		const FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(SprintEffect);
		SprintEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, Spec);
	}
}

void UBladeGameplayAbility_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (SprintEffectHandle.IsValid())
	{
		if (UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo())
		{
			OwnerASC->RemoveActiveGameplayEffect(SprintEffectHandle);
		}
		
		SprintEffectHandle = FActiveGameplayEffectHandle();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
