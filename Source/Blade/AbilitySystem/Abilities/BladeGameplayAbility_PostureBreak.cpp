#include "BladeGameplayAbility_PostureBreak.h"

#include "BladeGameplayTags.h"

UBladeGameplayAbility_PostureBreak::UBladeGameplayAbility_PostureBreak()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(BladeGameplayTags::Ability_PostureBreak);
	SetAssetTags(Tags);
	
	ActivationOwnedTags.AddTag(BladeGameplayTags::State_PostureBroken);
	
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_PostureBroken);
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_Dead);
	
	CancelAbilitiesWithTag.AddTag(BladeGameplayTags::Ability);
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = BladeGameplayTags::Event_Combat_PostureBroken;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UBladeGameplayAbility_PostureBreak::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		
	if (!ensureMsgf(PostureBreakMontage, TEXT("No PostureBreakMontage specified for %s"), *GetNameSafe(this)))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	PlayMontageAndEndOnCompletion(PostureBreakMontage);
	
}

void UBladeGameplayAbility_PostureBreak::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!IsEndAbilityValid(Handle, ActorInfo)) return;
	
	if (ensureMsgf(ResetPostureEffect, TEXT("No ResetPosture specified for %s"), *GetNameSafe(this)))
	{
		const FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(ResetPostureEffect);
		if (Spec.IsValid())
		{
			ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, Spec);
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
