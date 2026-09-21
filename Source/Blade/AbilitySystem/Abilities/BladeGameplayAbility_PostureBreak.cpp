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
