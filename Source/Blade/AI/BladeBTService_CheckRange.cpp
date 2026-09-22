#include "BladeBTService_CheckRange.h"

#include "AIController.h"
#include "BladeGameTypes.h"
#include "BehaviorTree/BlackboardComponent.h"

UBladeBTService_CheckRange::UBladeBTService_CheckRange()
{
	TargetActorKey.SelectedKeyName = NAME_TargetActor;
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBladeBTService_CheckRange, TargetActorKey), AActor::StaticClass());
	
	WithinRangeKey.SelectedKeyName = NAME_WithinRange;
	WithinRangeKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBladeBTService_CheckRange, WithinRangeKey));
}

void UBladeBTService_CheckRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	check(BBComp);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;
		
	APawn* OwningPawn = AIController->GetPawn();
	if (!OwningPawn) return;
	
	AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor)
	{
		BBComp->SetValueAsBool(WithinRangeKey.SelectedKeyName, false);
		return;
	}
	
	const float DistanceTo = FVector::Dist(TargetActor->GetActorLocation(), OwningPawn->GetActorLocation());
	BBComp->SetValueAsBool(WithinRangeKey.SelectedKeyName, DistanceTo < MaxRange);
}
