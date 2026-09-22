#include "BladeBTTask_CancelAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"

UBladeBTTask_CancelAbility::UBladeBTTask_CancelAbility()
{
	NodeName = "Cancel Ability By Tag";
}

EBTNodeResult::Type UBladeBTTask_CancelAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;
	
	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;
	
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AIPawn);
	if (!ASC) return EBTNodeResult::Failed;
	
	FGameplayTagContainer CancelTags(AbilityTag);
	ASC->CancelAbilities(&CancelTags);
	return EBTNodeResult::Succeeded;
}
