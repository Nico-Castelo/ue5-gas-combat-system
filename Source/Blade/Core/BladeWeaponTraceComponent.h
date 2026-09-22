#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BladeWeaponTraceComponent.generated.h"

class UStaticMeshComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLADE_API UBladeWeaponTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBladeWeaponTraceComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void BeginPlay() override;
	
	void StartTrace();
	
	void StopTrace();
	
protected:
	
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> WeaponMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	FName TraceStartSocketName = TEXT("TraceStart");

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	FName TraceEndSocketName = TEXT("TraceEnd");
	
	UPROPERTY(EditDefaultsOnly, Category = "Trace", meta = (ClampMin = "0.0"))
	float TraceRadius = 12.0f;
	
	UPROPERTY()
	TSet<TObjectPtr<AActor>> HitActors; 
};
