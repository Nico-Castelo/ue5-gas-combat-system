#include "BladeAttributeSet.h"

#include "AbilitySystemGlobals.h"
#include "BladeGameplayTags.h"
#include "GameplayEffectExtension.h"

UBladeAttributeSet::UBladeAttributeSet()
{
	
}

void UBladeAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		
		UAbilitySystemComponent* OwnerASC = GetOwningAbilitySystemComponent();
		if (!OwnerASC) return;
		
		if (GetHealth() <= 0.0f && !OwnerASC->HasMatchingGameplayTag(BladeGameplayTags::State_Dead))
		{
			FGameplayEventData DeathEventData;
			DeathEventData.Instigator = Data.EffectSpec.GetContext().GetInstigator();
			DeathEventData.Target = GetOwningActor();
			OwnerASC->HandleGameplayEvent(BladeGameplayTags::Event_Combat_Death, &DeathEventData);
		}
	}
	
	if (Data.EvaluatedData.Attribute == GetPostureAttribute())
	{
		SetPosture(FMath::Clamp(GetPosture(), 0.0f, GetMaxPosture()));
	}
}
