#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "BladeCharacterBase.generated.h"

class UMotionWarpingComponent;
class UGameplayEffect;
struct FOnAttributeChangeData;
class UBladeWeaponTraceComponent;
class UGameplayAbility;
class UBladeAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class BLADE_API ABladeCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	ABladeCharacterBase();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }
	
	virtual void PossessedBy(AController* NewController) override;
	
	void StartHitStop(float Duration);
	
	virtual AActor* GetCombatTarget() const;
	
protected:
	
	void EndHitStop();
	
	FTimerHandle HitStopTimerHandle;
	
	void OnMoveSpeedChanged(const FOnAttributeChangeData& Data);
	
	void Death(const FGameplayTag Tag, int32 NewCount);
	
	UPROPERTY(VisibleAnywhere, Category = "Ability System")
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	UPROPERTY(VisibleAnywhere, Category = "Motion Warping")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Ability System")
	TObjectPtr<UBladeAttributeSet> AttributeSet;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability System")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability System")
	TArray<TSubclassOf<UGameplayEffect>> DefaultAttributeEffects;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	TObjectPtr<UBladeWeaponTraceComponent> TraceComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Hit Stop", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float HitStopTimeDilation = 0.05f;
	
public:
	
	UStaticMeshComponent* GetWeaponMesh() { return WeaponMesh; }
};
