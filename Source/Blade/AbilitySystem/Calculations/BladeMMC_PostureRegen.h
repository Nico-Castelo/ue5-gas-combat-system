#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "BladeMMC_PostureRegen.generated.h"


UCLASS()
class BLADE_API UBladeMMC_PostureRegen : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	
	UBladeMMC_PostureRegen();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
protected:
	
	FGameplayEffectAttributeCaptureDefinition HealthDefinition;
	
	FGameplayEffectAttributeCaptureDefinition MaxHealthDefinition;
};
