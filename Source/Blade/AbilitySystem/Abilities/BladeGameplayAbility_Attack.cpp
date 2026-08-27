// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeGameplayAbility_Attack.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Blade.h"
#include "BladeGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Attributes/BladeAttributeSet.h"
#include "Core/BladeWeaponTraceComponent.h"

UBladeGameplayAbility_Attack::UBladeGameplayAbility_Attack()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(BladeGameplayTags::Ability_Attack);
	SetAssetTags(Tags);
	
	ActivationOwnedTags.AddTag(BladeGameplayTags::State_Attacking);
	
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_Attacking);
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_Evading);
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_HitReacting);
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_Dead);
	
	CancelAbilitiesWithTag.AddTag(BladeGameplayTags::Ability_Block);
}

void UBladeGameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(BladeGameplayTags::State_Attacking_Committed);
	
	UAbilityTask_WaitGameplayEvent* HitWindowBegin = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, BladeGameplayTags::Event_Montage_HitWindow_Begin, nullptr, true);
	HitWindowBegin->EventReceived.AddDynamic(this, &UBladeGameplayAbility_Attack::OnHitWindowBegin);
	HitWindowBegin->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* HitWindowEnd = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, BladeGameplayTags::Event_Montage_HitWindow_End, nullptr, true);
	HitWindowEnd->EventReceived.AddDynamic(this, &UBladeGameplayAbility_Attack::OnHitWindowEnd);
	HitWindowEnd->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* WaitRecover = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, BladeGameplayTags::Event_Montage_Recover, nullptr, true);
	WaitRecover->EventReceived.AddDynamic(this, &UBladeGameplayAbility_Attack::OnRecoveryStarted);
	WaitRecover->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* WeaponHit = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, BladeGameplayTags::Event_Combat_HitDealt, nullptr, false);
	WeaponHit->EventReceived.AddDynamic(this, &UBladeGameplayAbility_Attack::OnWeaponHit);
	WeaponHit->ReadyForActivation();
	
	PlayMontageAndEndOnCompletion(AttackMontage, Rate, RootMotionScale);
	
	UE_LOG(LogGame, Verbose, TEXT("Attack activated on %s"), *GetNameSafe(GetAvatarActorFromActorInfo()));
}

void UBladeGameplayAbility_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UBladeWeaponTraceComponent* Trace = GetWeaponTraceComponent())
	{
		Trace->StopTrace();
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC && ASC->HasMatchingGameplayTag(BladeGameplayTags::State_Attacking_Committed))
	{
		ASC->RemoveLooseGameplayTag(BladeGameplayTags::State_Attacking_Committed);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBladeGameplayAbility_Attack::OnRecoveryStarted(FGameplayEventData Payload)
{
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(BladeGameplayTags::State_Attacking_Committed);
}

void UBladeGameplayAbility_Attack::OnHitWindowBegin(FGameplayEventData Payload)
{
	if (UBladeWeaponTraceComponent* Trace = GetWeaponTraceComponent())
	{
		Trace->StartTrace();
	}
}

void UBladeGameplayAbility_Attack::OnHitWindowEnd(FGameplayEventData Payload)
{
	if (UBladeWeaponTraceComponent* Trace = GetWeaponTraceComponent())
	{
		Trace->StopTrace();
	}
}

void UBladeGameplayAbility_Attack::OnWeaponHit(FGameplayEventData Payload)
{
	if (!ensureMsgf(DamageEffect, TEXT("No Damage Effect specified for %s"), *GetNameSafe(this))) return;
	if (!ensureMsgf(PostureDamageEffect, TEXT("No Posture Damage Effect specified for %s"), *GetNameSafe(this))) return;

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC) return;
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Payload.Target);
	if (!TargetASC) return;

	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffect);
	if (!DamageSpecHandle.IsValid()) return;
	SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);
	
	FGameplayEffectSpecHandle PostureSpecHandle = MakeOutgoingGameplayEffectSpec(PostureDamageEffect);
	if (!PostureSpecHandle.IsValid()) return;
	SourceASC->ApplyGameplayEffectSpecToTarget(*PostureSpecHandle.Data.Get(), TargetASC);
	
	FGameplayEventData HitReceivedPayload;
	HitReceivedPayload.Instigator = GetAvatarActorFromActorInfo();
	HitReceivedPayload.Target = Payload.Target;
	TargetASC->HandleGameplayEvent(BladeGameplayTags::Event_Combat_HitReceived, &HitReceivedPayload);

	UE_LOG(LogGame, Verbose, TEXT("Hit resolved: %s -> %s - Health now %.0f, Posture now %.0f"),
		*GetNameSafe(GetAvatarActorFromActorInfo()), *GetNameSafe(Payload.Target),
		TargetASC->GetNumericAttribute(UBladeAttributeSet::GetHealthAttribute()),
		TargetASC->GetNumericAttribute(UBladeAttributeSet::GetPostureAttribute()));
}

UBladeWeaponTraceComponent* UBladeGameplayAbility_Attack::GetWeaponTraceComponent() const
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	return Avatar ? Avatar->FindComponentByClass<UBladeWeaponTraceComponent>() : nullptr;
}
