// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BladeGameplayAbility.h"
#include "BladeGameplayAbility_Evade.generated.h"

class UAnimMontage;

UCLASS()
class BLADE_API UBladeGameplayAbility_Evade : public UBladeGameplayAbility
{
	GENERATED_BODY()

public:

	UBladeGameplayAbility_Evade();

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void OnMontageBlendOut() override;
	
	UFUNCTION()
	void OnRecoveryStarted(FGameplayEventData Payload);
	
	// Order matches DirectionIndex: F, FR, R, BR, B, BL, L, FL
	UPROPERTY(EditDefaultsOnly, Category = "Evade")
	TArray<TObjectPtr<UAnimMontage>> DirectionalEvadeMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Evade")
	TObjectPtr<UAnimMontage> EvadeMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Evade", meta = (ClampMin = "0.0"))
	float RootMotionScale = 1.25f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Evade", meta = (ClampMin = "0.0"))
	float Rate = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Evade", meta = (ClampMin = "0.0"))
	float RecoveryCancelBlendOut = 0.1f;
};
