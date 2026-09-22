#include "BladeBTTask_ActivateAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"

UBladeBTTask_ActivateAbility::UBladeBTTask_ActivateAbility()
{
	NodeName = "Activate Ability By Tag";
}

EBTNodeResult::Type UBladeBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;
	
	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;
	
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AIPawn);
	if (!ASC) return EBTNodeResult::Failed;
	
	return ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityTag)) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

FString UBladeBTTask_ActivateAbility::GetStaticDescription() const
{
	return FString::Printf(TEXT("Activate %s"), *AbilityTag.ToString());
}
