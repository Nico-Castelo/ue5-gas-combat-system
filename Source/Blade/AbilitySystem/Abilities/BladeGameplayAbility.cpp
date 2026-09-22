#include "BladeGameplayAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UBladeGameplayAbility::UBladeGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UBladeGameplayAbility::PlayMontageAndEndOnCompletion(UAnimMontage* Montage, float Rate, float RootMotionScale, FName StartSection)
{
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		Montage,
		Rate,
		StartSection,
		true,
		RootMotionScale
	);

	MontageTask->OnCompleted.AddDynamic(this, &UBladeGameplayAbility::OnMontageCompleted);
	MontageTask->OnBlendOut.AddDynamic(this, &UBladeGameplayAbility::OnMontageBlendOut);
	MontageTask->OnInterrupted.AddDynamic(this, &UBladeGameplayAbility::OnMontageCancelled);
	MontageTask->OnCancelled.AddDynamic(this, &UBladeGameplayAbility::OnMontageCancelled);

	MontageTask->ReadyForActivation();
}

void UBladeGameplayAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UBladeGameplayAbility::OnMontageBlendOut()
{
	
}

void UBladeGameplayAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
