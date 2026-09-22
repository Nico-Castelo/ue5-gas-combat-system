#pragma once

#include "CoreMinimal.h"
#include "BladeGameplayAbility.h"
#include "BladeGameplayAbility_Block.generated.h"

UCLASS()
class BLADE_API UBladeGameplayAbility_Block : public UBladeGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	UBladeGameplayAbility_Block();
	
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void OnBlockHit(FGameplayEventData Payload);
	
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	TObjectPtr<UAnimMontage> BlockHitMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	TSubclassOf<UGameplayEffect> SlowEffect;
	
	FActiveGameplayEffectHandle SlowEffectHandle;
};
