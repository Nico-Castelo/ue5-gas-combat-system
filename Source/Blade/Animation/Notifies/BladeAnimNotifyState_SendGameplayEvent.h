#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "BladeAnimNotifyState_SendGameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API UBladeAnimNotifyState_SendGameplayEvent : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	
	UPROPERTY(EditAnywhere, meta= (Categories = "Event"))
	FGameplayTag BeginEventTag;
	
	UPROPERTY(EditAnywhere, meta= (Categories = "Event"))
	FGameplayTag EndEventTag;
};
