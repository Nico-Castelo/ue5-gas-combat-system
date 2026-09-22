#include "BladeCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "BladeGameplayTags.h"
#include "BladeWeaponTraceComponent.h"
#include "MotionWarpingComponent.h"
#include "AbilitySystem/Attributes/BladeAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ABladeCharacterBase::ABladeCharacterBase()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	
	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpComp"));
	
	AttributeSet = CreateDefaultSubobject<UBladeAttributeSet>(TEXT("AttributeSet"));
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("weapon_r1Socket"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	TraceComponent = CreateDefaultSubobject<UBladeWeaponTraceComponent>(TEXT("TraceComp"));
}

void ABladeCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	ASC->InitAbilityActorInfo(this, this);
	
	ASC->GetGameplayAttributeValueChangeDelegate(UBladeAttributeSet::GetMoveSpeedAttribute())
	.AddUObject(this, &ABladeCharacterBase::OnMoveSpeedChanged);
	
	ASC->RegisterGameplayTagEvent(BladeGameplayTags::State_Dead).AddUObject(this, &ABladeCharacterBase::Death);

	for (const TSubclassOf<UGameplayEffect>& EffectClass : DefaultAttributeEffects)
	{
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, 1.0f, ASC->MakeEffectContext());
		if (ensureMsgf(SpecHandle.IsValid(), TEXT("%s: GE_Init_Attributes not set in Blueprint"), *GetNameSafe(this)))
		{
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
	
	GetCharacterMovement()->MaxWalkSpeed = AttributeSet->GetMoveSpeed();
	
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass));
	}
}

AActor* ABladeCharacterBase::GetCombatTarget() const
{
	return nullptr;
}

void ABladeCharacterBase::OnMoveSpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

void ABladeCharacterBase::Death(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount == 0) return;
	
	AController* OwningController = GetController();
	
	if (!ensure(OwningController)) return;
	
	OwningController->SetIgnoreMoveInput(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}