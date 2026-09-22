// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeGameplayAbility_HitReact.h"

#include "AbilitySystemComponent.h"
#include "Blade.h"
#include "BladeGameplayTags.h"
#include "Animation/AnimMontage.h"

UBladeGameplayAbility_HitReact::UBladeGameplayAbility_HitReact()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(BladeGameplayTags::Ability_HitReact);
	SetAssetTags(Tags);

	ActivationOwnedTags.AddTag(BladeGameplayTags::State_HitReacting);
	
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_Dead);
	ActivationBlockedTags.AddTag(BladeGameplayTags::State_Blocking);

	CancelAbilitiesWithTag.AddTag(BladeGameplayTags::Ability_Attack);
	CancelAbilitiesWithTag.AddTag(BladeGameplayTags::Ability_Evade);
	CancelAbilitiesWithTag.AddTag(BladeGameplayTags::Ability_PostureBreak);

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = BladeGameplayTags::Event_Combat_HitReceived;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	bRetriggerInstancedAbility = true;
	AbilityTriggers.Add(TriggerData);
}

void UBladeGameplayAbility_HitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!TriggerEventData || !TriggerEventData->Instigator)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const EHitDirection Direction = CalculateHitDirection(TriggerEventData->Instigator);
	const FName Section = DirectionSectionName(Direction);

	if (!ensureMsgf(HitReactMontage, TEXT("No HitReactMontage specified for %s"), *GetNameSafe(this)) ||
		!ensureMsgf(HitReactMontage->IsValidSectionName(Section), TEXT("%s has no section named %s"), *GetNameSafe(HitReactMontage), *Section.ToString()))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	PlayMontageAndEndOnCompletion(HitReactMontage, Rate, RootMotionScale, Section);

	UE_LOG(LogGame, Verbose, TEXT("HitReact: %s on %s from %s"),
		*Section.ToString(), *GetNameSafe(GetAvatarActorFromActorInfo()), *GetNameSafe(TriggerEventData->Instigator));
}

EHitDirection UBladeGameplayAbility_HitReact::CalculateHitDirection(const AActor* Attacker) const
{
	const AActor* Avatar = GetAvatarActorFromActorInfo();
	const FVector ToAttacker = (Attacker->GetActorLocation() - Avatar->GetActorLocation()).GetSafeNormal2D();

	const float ForwardDot = FVector::DotProduct(ToAttacker, Avatar->GetActorForwardVector());

	if (ForwardDot > 0.5f)  return EHitDirection::Forward;
	if (ForwardDot < -0.5f) return EHitDirection::Backward;

	const float RightDot = FVector::DotProduct(ToAttacker, Avatar->GetActorRightVector());
	return RightDot > 0.0f ? EHitDirection::Right : EHitDirection::Left;
}

FName UBladeGameplayAbility_HitReact::DirectionSectionName(EHitDirection Direction)
{
	static const FName Forward(TEXT("Forward"));
	static const FName Backward(TEXT("Backward"));
	static const FName Left(TEXT("Left"));
	static const FName Right(TEXT("Right"));

	switch (Direction)
	{
	case EHitDirection::Forward:  return Forward;
	case EHitDirection::Backward: return Backward;
	case EHitDirection::Left:     return Left;
	case EHitDirection::Right:    return Right;
	}

	return Forward;
}
