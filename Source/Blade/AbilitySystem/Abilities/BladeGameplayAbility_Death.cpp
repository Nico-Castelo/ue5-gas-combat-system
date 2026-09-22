#include "BladeGameplayAbility_Death.h"

#include "AbilitySystemComponent.h"
#include "BladeGameplayTags.h"

UBladeGameplayAbility_Death::UBladeGameplayAbility_Death()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(BladeGameplayTags::Ability_Death);
	SetAssetTags(Tags);
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = BladeGameplayTags::Event_Combat_Death;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
	
	CancelAbilitiesWithTag.AddTag(BladeGameplayTags::Ability);
	BlockAbilitiesWithTag.AddTag(BladeGameplayTags::Ability);
}

void UBladeGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	PlayMontageAndEndOnCompletion(DeathMontage);
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC || !ensure(DeathGameplayEffect)) return;
	
	FGameplayEffectSpecHandle DeathGameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DeathGameplayEffect);
	if ( !DeathGameplayEffectSpecHandle.IsValid() ) return;
	
	ASC->ApplyGameplayEffectSpecToSelf(*DeathGameplayEffectSpecHandle.Data.Get());
}
