#include "BladeAnimNotifyState_SendGameplayEvent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbilityTypes.h"

void UBladeAnimNotifyState_SendGameplayEvent::NotifyBegin(USkeletalMeshComponent* MeshComp,
                                                          UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
	
	if (!Owner || (!BeginEventTag.IsValid())) return;
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
	{
		FGameplayEventData Payload;
		ASC->HandleGameplayEvent(BeginEventTag, &Payload);
	}
}

void UBladeAnimNotifyState_SendGameplayEvent::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;

	if (!Owner || !EndEventTag.IsValid()) return;

	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
	{
		FGameplayEventData Payload;
		ASC->HandleGameplayEvent(EndEventTag, &Payload);
	}
}
