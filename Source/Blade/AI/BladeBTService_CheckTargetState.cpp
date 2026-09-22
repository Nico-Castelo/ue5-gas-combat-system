#include "BladeBTService_CheckTargetState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "BladeGameplayTags.h"
#include "BladeGameTypes.h"
#include "BehaviorTree/BlackboardComponent.h"

UBladeBTService_CheckTargetState::UBladeBTService_CheckTargetState()
{
	TargetActorKey.SelectedKeyName = NAME_TargetActor;
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBladeBTService_CheckTargetState, TargetActorKey), AActor::StaticClass());
	
	OutStateKey.SelectedKeyName = NAME_TargetIsAttacking;
	OutStateKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBladeBTService_CheckTargetState, OutStateKey));

	StateTag = BladeGameplayTags::State_Attacking;
}

void UBladeBTService_CheckTargetState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;
	
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor)
	{
		BlackboardComp->SetValueAsBool(OutStateKey.SelectedKeyName, false);
		return;
	}
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
	if (!TargetASC)
	{
		BlackboardComp->SetValueAsBool(OutStateKey.SelectedKeyName, false);
		return;
	}
	
	BlackboardComp->SetValueAsBool(OutStateKey.SelectedKeyName, TargetASC->HasMatchingGameplayTag(StateTag));
}
