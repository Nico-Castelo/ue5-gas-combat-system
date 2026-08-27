// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeLockOnComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Blade.h"
#include "BladeGameplayTags.h"
#include "BladePlayerCharacter.h"
#include "BladePlayerController.h"
#include "Engine/OverlapResult.h"


UBladeLockOnComponent::UBladeLockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UBladeLockOnComponent::ToggleLockOn()
{
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	
	if (CurrentTarget != nullptr || ASC->HasMatchingGameplayTag(BladeGameplayTags::State_Dead))
	{
		CurrentTarget = nullptr;
		SetComponentTickEnabled(false);
	}
	else
	{
		FindBestTarget();
	}
}

void UBladeLockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!CurrentTarget)
	{
		SetComponentTickEnabled(false);
		return;
	}
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(CurrentTarget);
	if (!TargetASC || TargetASC->HasMatchingGameplayTag(BladeGameplayTags::State_Dead))
	{
		CurrentTarget = nullptr;
		SetComponentTickEnabled(false);
		return;
	}
	
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	AController* Controller = OwnerPawn ? OwnerPawn->GetController() : nullptr;
	if (!Controller) return;
		
	FVector Direction = (CurrentTarget->GetActorLocation() - GetOwner()->GetActorLocation());
	
	FRotator LookRotation = Direction.Rotation();
	LookRotation.Pitch += PitchOffset;
	
	FRotator SmoothedRotation = FMath::RInterpTo(Controller->GetControlRotation(), LookRotation, DeltaTime, RotationSpeed);
	
	Controller->SetControlRotation(SmoothedRotation);
}

void UBladeLockOnComponent::FindBestTarget()
{
	ABladePlayerCharacter* PlayerCharacter = Cast<ABladePlayerCharacter>(GetOwner());
	check(PlayerCharacter);
	
	ABladePlayerController* PC = Cast<ABladePlayerController>(PlayerCharacter->GetController());
	if (!PC) return;
	
	FVector Center = GetOwner()->GetActorLocation();
	
	FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
	FVector CameraForward = PC->PlayerCameraManager->GetCameraRotation().Vector();
	
	FCollisionShape Shape;
	Shape.SetSphere(TargetRadius);
	
	TArray<FOverlapResult> OverlapResults;
	GetWorld()->OverlapMultiByChannel(OverlapResults, Center, FQuat::Identity, ECC_LockOnTarget, Shape);
	
	AActor* BestTargetActor = nullptr;
	
	float BestDot = 0.3f;
	
	for (FOverlapResult& Overlap : OverlapResults)
	{
		AActor* Target = Overlap.GetActor();
		if (!Target) continue;
		
		UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);

		if (!TargetASC || TargetASC->HasMatchingGameplayTag(BladeGameplayTags::State_Dead)) continue;
		
		FVector ToTarget = (Target->GetActorLocation() - CameraLocation).GetSafeNormal();
		
		float Dot = FVector::DotProduct(CameraForward, ToTarget);

		if (Dot > BestDot)
		{
			BestDot = Dot;
			BestTargetActor = Target;
		}
	}
	
	CurrentTarget = BestTargetActor;
	
	if (CurrentTarget)
	{
		SetComponentTickEnabled(true);
	}
}

