#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GameplayTagContainer.h"
#include "BladeBTService_CheckTargetState.generated.h"

UCLASS()
class BLADE_API UBladeBTService_CheckTargetState : public UBTService
{
	GENERATED_BODY()
	
public:
	
	UBladeBTService_CheckTargetState();
	
protected:
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetActorKey;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector OutStateKey;
	
	UPROPERTY(EditAnywhere, Category = "AI",  meta= (Categories = "State"))
	FGameplayTag StateTag;
};
