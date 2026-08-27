// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BladeLockOnComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLADE_API UBladeLockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UBladeLockOnComponent();

	void ToggleLockOn();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:

	void FindBestTarget();
	
	UPROPERTY(EditDefaultsOnly, Category= "Lock")
	float PitchOffset = -15.0f;
	
	UPROPERTY(EditDefaultsOnly, Category= "Lock")
	float RotationSpeed = 10.0f;

	UPROPERTY(Transient)
	TObjectPtr<AActor> CurrentTarget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Lock")
	float TargetRadius = 500.0f;
	
};
