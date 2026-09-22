#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BladeAnimNotify_SendGameplayEvent.generated.h"

UCLASS()
class BLADE_API UBladeAnimNotify_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	
	UPROPERTY(EditAnywhere, meta = (Categories = "Event"))
	FGameplayTag EventTag;
};
