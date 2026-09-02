// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BladeGameplayAbility.h"
#include "BladeGameplayAbility_HitReact.generated.h"

enum class EHitDirection : uint8
{
	Forward,
	Backward,
	Left,
	Right
};

UCLASS()
class BLADE_API UBladeGameplayAbility_HitReact : public UBladeGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	UBladeGameplayAbility_HitReact();
	
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	EHitDirection CalculateHitDirection(const AActor* Attacker) const;
	
	static FName DirectionSectionName(EHitDirection Direction);
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (ClampMin = "0.0"))
	float RootMotionScale = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (ClampMin = "0.0"))
	float Rate = 1.0f;
};
