#pragma once

#include "CoreMinimal.h"
#include "BladeGameplayAbility.h"
#include "BladeGameplayAbility_Attack.generated.h"

class UBladeWeaponTraceComponent;

UCLASS()
class BLADE_API UBladeGameplayAbility_Attack : public UBladeGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	UBladeGameplayAbility_Attack();
	
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	void AdvanceCombo(bool bJumpNow);
	
	UFUNCTION()
	void OnRecoveryStarted(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnHitWindowBegin(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnComboWindowBegin(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnComboWindowEnd(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnHitWindowEnd(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnWeaponHit(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnComboQueued(FGameplayEventData Payload);
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TArray<FName> ComboSections;
	
	int32 ComboIndex = 0;
	
	bool bComboWindowOpen = false;
	
	bool bComboInputQueued = false;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<UGameplayEffect> DamageEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<UGameplayEffect> PostureDamageEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack", meta = (ClampMin = "0.0"))
	float RootMotionScale = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack", meta = (ClampMin = "0.0"))
	float Rate = 1.6f;
	
	UBladeWeaponTraceComponent* GetWeaponTraceComponent() const;
};
