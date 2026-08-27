// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeAIController.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "BladeGameplayTags.h"
#include "BladeGameTypes.h"
#include "BrainComponent.h"
#include "GameplayTagContainer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


ABladeAIController::ABladeAIController()
{
	
}

void ABladeAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (!ensureMsgf(BehaviourTree, TEXT("BehaviourTree not assigned to %s"), *GetNameSafe(this))) return;
		
	RunBehaviorTree(BehaviourTree);
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	check(PlayerPawn);
	
	GetBlackboardComponent()->SetValueAsObject(NAME_TargetActor, PlayerPawn);
	
	SetFocus(PlayerPawn);
	
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InPawn);
	check(ASC);
	
	ASC->RegisterGameplayTagEvent(BladeGameplayTags::State_Dead).AddUObject(this, &ABladeAIController::OnPawnDeath);
}

void ABladeAIController::OnPawnDeath(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount <= 0) return;
	
	if (BrainComponent)
	{
		BrainComponent->StopLogic(TEXT("Dead"));
	}
	
	ClearFocus(EAIFocusPriority::Gameplay);
}

