#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BladeAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class BLADE_API UBladeAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	
	UBladeAttributeSet();
	
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	ATTRIBUTE_ACCESSORS(UBladeAttributeSet, Health)
	ATTRIBUTE_ACCESSORS(UBladeAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS(UBladeAttributeSet, Posture)
	ATTRIBUTE_ACCESSORS(UBladeAttributeSet, MaxPosture)
	ATTRIBUTE_ACCESSORS(UBladeAttributeSet, MoveSpeed)
	
protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Posture;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxPosture;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MoveSpeed;
};
