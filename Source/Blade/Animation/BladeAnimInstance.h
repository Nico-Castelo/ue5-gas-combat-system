// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BladeAnimInstance.generated.h"

class UAbilitySystemComponent;
class ACharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class BLADE_API UBladeAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeInitializeAnimation() override;
		
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float GroundSpeed = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsFalling = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float VerticalVelocity = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Direction = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsBlocking = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsLockedOn = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsSprinting = false;
	
	UPROPERTY(Transient)
	TObjectPtr<ACharacter> OwnerCharacter;
	
	UPROPERTY(Transient)
	TObjectPtr<UCharacterMovementComponent> OwnerMovementComponent;
	
	UPROPERTY(Transient)
	TObjectPtr<UAbilitySystemComponent> OwnerASC;
};
