#pragma once

#include "CoreMinimal.h"
#include "BladeGameplayAbility.h"
#include "BladeGameplayAbility_Deflect.generated.h"

UCLASS()
class BLADE_API UBladeGameplayAbility_Deflect : public UBladeGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	UBladeGameplayAbility_Deflect();
	
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Deflect")
	TSubclassOf<UGameplayEffect> DeflectWindowEffect;
};
