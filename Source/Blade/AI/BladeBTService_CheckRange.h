#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BladeBTService_CheckRange.generated.h"

UCLASS()
class BLADE_API UBladeBTService_CheckRange : public UBTService
{
	GENERATED_BODY()
	
public:
	
	UBladeBTService_CheckRange();

protected:
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetActorKey;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector WithinRangeKey;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxRange = 200.0f;
};
