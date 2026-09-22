#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "BladeBTTask_ActivateAbility.generated.h"

UCLASS()
class BLADE_API UBladeBTTask_ActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	
	UBladeBTTask_ActivateAbility();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual FString GetStaticDescription() const override;
	
protected:
	
	UPROPERTY(EditAnywhere,  Category = "Ability", meta = (Categories = "Ability"))
	FGameplayTag AbilityTag;
};
