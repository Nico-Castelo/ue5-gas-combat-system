#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BladeGameplayAbility.generated.h"

class UAnimMontage;

UCLASS(Abstract)
class BLADE_API UBladeGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	UBladeGameplayAbility();
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (ClampMin = "0.0"))
	float RootMotionScale = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (ClampMin = "0.0"))
	float Rate = 1.0f;
	
	void PlayMontageAndEndOnCompletion(UAnimMontage* Montage, FName StartSection = NAME_None);

	UFUNCTION()
	virtual void OnMontageCompleted();

	UFUNCTION()
	virtual void OnMontageBlendOut();

	UFUNCTION()
	virtual void OnMontageCancelled();
};
