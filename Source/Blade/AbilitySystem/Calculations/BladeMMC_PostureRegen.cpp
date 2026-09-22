#include "BladeMMC_PostureRegen.h"

#include "AbilitySystem/Attributes/BladeAttributeSet.h"

UBladeMMC_PostureRegen::UBladeMMC_PostureRegen()
{
	HealthDefinition = FGameplayEffectAttributeCaptureDefinition(
		UBladeAttributeSet::GetHealthAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false);
	
	MaxHealthDefinition = FGameplayEffectAttributeCaptureDefinition(
		UBladeAttributeSet::GetMaxHealthAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false);
	
	RelevantAttributesToCapture.Add(HealthDefinition);
	RelevantAttributesToCapture.Add(MaxHealthDefinition);
}

float UBladeMMC_PostureRegen::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	
	float HealthDefMagnitude = 0.0f;
	
	float MaxHealthDefMagnitude = 0.0f;
	
	GetCapturedAttributeMagnitude(HealthDefinition, Spec, EvaluationParameters, HealthDefMagnitude);
	
	GetCapturedAttributeMagnitude(MaxHealthDefinition, Spec, EvaluationParameters, MaxHealthDefMagnitude);
	
	if (!ensureMsgf((MaxHealthDefMagnitude > 0.0f), TEXT("PostureRegen MMC: MaxHealth <= 0")))
	{
		return 0.0f;
	}
	
	return HealthDefMagnitude / MaxHealthDefMagnitude;
}
