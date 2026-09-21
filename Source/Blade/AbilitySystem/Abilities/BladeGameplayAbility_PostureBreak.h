#pragma once

#include "CoreMinimal.h"
#include "BladeGameplayAbility.h"
#include "BladeGameplayAbility_PostureBreak.generated.h"

UCLASS()
class BLADE_API UBladeGameplayAbility_PostureBreak : public UBladeGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	UBladeGameplayAbility_PostureBreak();
	
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> PostureBreakMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> KnockdownRecoverEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float KnockdownDuration = 5.0f;
};
