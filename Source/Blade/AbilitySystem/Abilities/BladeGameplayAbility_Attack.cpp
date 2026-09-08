#include "BladeGameplayAbility_Attack.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Blade.h"
#include "BladeGameplayTags.h"
#include "MotionWarpingComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Attributes/BladeAttributeSet.h"
#include "Core/BladeCharacterBase.h"
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
	
	ABladeCharacterBase* Character = Cast<ABladeCharacterBase>(GetAvatarActorFromActorInfo());
	check(Character);
	
	const AActor* Target = Character->GetCombatTarget();
	UMotionWarpingComponent* MotionWarpingComp = GetMotionWarpingComponent();
	
	if (Target && MotionWarpingComp)
	{
		MotionWarpingComp->AddOrUpdateWarpTargetFromComponent(WarpTargetName, Target->GetRootComponent(), NAME_None, true, EWarpTargetLocationOffsetDirection::TargetsForwardVector);
	}
	
	ComboIndex = 0;
	bComboInputQueued = false;
	bComboWindowOpen = false;
	
	if (!ensureMsgf(AttackMontage, TEXT("No AttackMontage specified for %s"), *GetNameSafe(this)))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	for (const FName SectionName : ComboSections)
	{
		if (!ensureMsgf(AttackMontage->IsValidSectionName(SectionName), TEXT("Invalid section name %s in AttackMontage for %s"), *SectionName.ToString(), *GetNameSafe(AttackMontage)))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}
	}
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(BladeGameplayTags::State_Attacking_Committed);
	
	UAbilityTask_WaitGameplayEvent* WaitCombo = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, BladeGameplayTags::Event_Input_ComboQueued, nullptr, false);
	WaitCombo->EventReceived.AddDynamic(this, &UBladeGameplayAbility_Attack::OnComboQueued);
	WaitCombo->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* ComboWindowBegin = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, BladeGameplayTags::Event_Montage_ComboWindow_Begin, nullptr, false);
	ComboWindowBegin->EventReceived.AddDynamic(this, &UBladeGameplayAbility_Attack::OnComboWindowBegin);
	ComboWindowBegin->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* ComboWindowEnd = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, BladeGameplayTags::Event_Montage_ComboWindow_End, nullptr, false);
	ComboWindowEnd->EventReceived.AddDynamic(this, &UBladeGameplayAbility_Attack::OnComboWindowEnd);
	ComboWindowEnd->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* HitWindowBegin = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, BladeGameplayTags::Event_Montage_HitWindow_Begin, nullptr, false);
	HitWindowBegin->EventReceived.AddDynamic(this, &UBladeGameplayAbility_Attack::OnHitWindowBegin);
	HitWindowBegin->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* HitWindowEnd = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, BladeGameplayTags::Event_Montage_HitWindow_End, nullptr, false);
	HitWindowEnd->EventReceived.AddDynamic(this, &UBladeGameplayAbility_Attack::OnHitWindowEnd);
	HitWindowEnd->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* WaitRecover = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, BladeGameplayTags::Event_Montage_Recover, nullptr, false);
	WaitRecover->EventReceived.AddDynamic(this, &UBladeGameplayAbility_Attack::OnRecoveryStarted);
	WaitRecover->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* WeaponHit = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, BladeGameplayTags::Event_Combat_HitDealt, nullptr, false);
	WeaponHit->EventReceived.AddDynamic(this, &UBladeGameplayAbility_Attack::OnWeaponHit);
	WeaponHit->ReadyForActivation();
	
	const FName StartSection = ComboSections.IsValidIndex(0) ? ComboSections[0] : NAME_None;
	PlayMontageAndEndOnCompletion(AttackMontage, Rate, RootMotionScale, StartSection);
	
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
	check(ASC);
	
	if (ASC->HasMatchingGameplayTag(BladeGameplayTags::State_Attacking_Committed))
	{
		ASC->RemoveLooseGameplayTag(BladeGameplayTags::State_Attacking_Committed);
	}
	
	if (UMotionWarpingComponent* MotionWarpingComp = GetMotionWarpingComponent())
	{
		MotionWarpingComp->RemoveWarpTarget(WarpTargetName);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBladeGameplayAbility_Attack::AdvanceCombo(bool bJumpNow)
{
	const int32 NextSection = ComboIndex + 1 < ComboSections.Num() ? ComboIndex + 1 : 0;
	bComboInputQueued = false;
	
	if (!ComboSections.IsValidIndex(NextSection)) return;
	
	if (bJumpNow)
	{
		MontageJumpToSection(ComboSections[NextSection]);
	}
	else
	{
		MontageSetNextSectionName(ComboSections[ComboIndex], ComboSections[NextSection]);
	}

	ComboIndex = NextSection;
	bComboWindowOpen = false;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	check(ASC);
	
	ASC->AddLooseGameplayTag(BladeGameplayTags::State_Attacking_Committed);
}

void UBladeGameplayAbility_Attack::OnRecoveryStarted(FGameplayEventData Payload)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	check(ASC);
	
	ASC->RemoveLooseGameplayTag(BladeGameplayTags::State_Attacking_Committed);
	
	if (bComboInputQueued)
	{
		AdvanceCombo(true);
	}
}

void UBladeGameplayAbility_Attack::OnHitWindowBegin(FGameplayEventData Payload)
{
	UBladeWeaponTraceComponent* TraceComp = GetWeaponTraceComponent();
	check(TraceComp);
	
	TraceComp->StartTrace();
}

void UBladeGameplayAbility_Attack::OnComboWindowBegin(FGameplayEventData Payload)
{
	bComboWindowOpen = true;
}

void UBladeGameplayAbility_Attack::OnComboWindowEnd(FGameplayEventData Payload)
{
	bComboWindowOpen = false;
}

void UBladeGameplayAbility_Attack::OnHitWindowEnd(FGameplayEventData Payload)
{
	UBladeWeaponTraceComponent* TraceComp = GetWeaponTraceComponent();
	check(TraceComp);
	
	TraceComp->StopTrace();
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

void UBladeGameplayAbility_Attack::OnComboQueued(FGameplayEventData Payload)
{
	if (!bComboWindowOpen) return;
	
	const bool bStillCommitted = GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(BladeGameplayTags::State_Attacking_Committed);
	
	if (bStillCommitted)
	{
		bComboInputQueued = true;
	}
	else
	{
		AdvanceCombo(true);
	}
}

UBladeWeaponTraceComponent* UBladeGameplayAbility_Attack::GetWeaponTraceComponent() const
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	return Avatar ? Avatar->FindComponentByClass<UBladeWeaponTraceComponent>() : nullptr;
}

UMotionWarpingComponent* UBladeGameplayAbility_Attack::GetMotionWarpingComponent() const
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	return Avatar ? Avatar->FindComponentByClass<UMotionWarpingComponent>() : nullptr;
}
