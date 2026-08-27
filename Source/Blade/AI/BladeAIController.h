// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BladeAIController.generated.h"

struct FGameplayTag;
class UBehaviorTree;

UCLASS()
class BLADE_API ABladeAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABladeAIController();

protected:
	
	virtual void OnPossess(APawn* InPawn) override;
	
	void OnPawnDeath(const FGameplayTag Tag, int32 NewCount);
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviourTree;
};
