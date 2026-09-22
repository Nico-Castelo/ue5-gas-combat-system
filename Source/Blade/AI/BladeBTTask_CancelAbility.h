#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "BladeBTTask_CancelAbility.generated.h"

UCLASS()
class BLADE_API UBladeBTTask_CancelAbility : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	
	UBladeBTTask_CancelAbility();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;	
	
protected:
	
	UPROPERTY(EditAnywhere,  Category = "Ability", meta = (Categories = "Ability"))
	FGameplayTag AbilityTag;
};
