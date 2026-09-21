#include "BladeGameplayAbility_Deflect.h"

#include "AbilitySystemComponent.h"
#include "BladeGameplayTags.h"

UBladeGameplayAbility_Deflect::UBladeGameplayAbility_Deflect()
{
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = BladeGameplayTags::Event_Input_BlockPressed;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);

	ActivationBlockedTags.AddTag(BladeGameplayTags::State_PostureBroken);
}

void UBladeGameplayAbility_Deflect::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (ensureMsgf(DeflectWindowEffect, TEXT("DeflectWindowEffect not set on %s"), *GetName()))
	{
		const FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(DeflectWindowEffect);
		if (Spec.IsValid())
		{
			ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, Spec);
		}
		else
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
