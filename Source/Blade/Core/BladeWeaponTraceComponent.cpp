#include "BladeWeaponTraceComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Blade.h"
#include "BladeCharacterBase.h"
#include "BladeGameplayTags.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "HAL/IConsoleManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"

static TAutoConsoleVariable<bool> CVarWeaponTraceDebugDraw(
	TEXT("blade.combat.WeaponTraceDebugDraw"),
	false,
	TEXT("Draw weapon sphere traces during attack hit windows."),
	ECVF_Cheat);

UBladeWeaponTraceComponent::UBladeWeaponTraceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UBladeWeaponTraceComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ABladeCharacterBase* Character = Cast<ABladeCharacterBase>(GetOwner());

	if (!ensureMsgf(Character, TEXT("%s: the owner is not a BladeCharacterBase"), *GetName())) return;

	WeaponMesh = Character->GetWeaponMesh();
}

void UBladeWeaponTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	const FVector Start = WeaponMesh->GetSocketLocation(TraceStartSocketName);
	const FVector End = WeaponMesh->GetSocketLocation(TraceEndSocketName);

	TArray<FHitResult> Hits;
	const bool bDebugDraw = CVarWeaponTraceDebugDraw.GetValueOnGameThread();
	const EDrawDebugTrace::Type DebugDrawType = bDebugDraw ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	UKismetSystemLibrary::SphereTraceMulti(this, Start, End, TraceRadius,
		UEngineTypes::ConvertToTraceType(ECC_Weapon), false, { GetOwner() },
		DebugDrawType, Hits, true, FLinearColor::Red, FLinearColor::Green, 2.0f);
	
	for (const FHitResult& Hit: Hits)
	{
		AActor* HitActor = Hit.GetActor();
		
		if (!HitActor || HitActors.Contains(HitActor)) continue;
		
		HitActors.Add(HitActor);
		
		FGameplayEventData Payload;	
		Payload.Instigator = GetOwner();
		Payload.Target = HitActor;
		
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
		{
			ASC->HandleGameplayEvent(BladeGameplayTags::Event_Combat_HitDealt, &Payload);
		}
	}
}

void UBladeWeaponTraceComponent::StartTrace()
{
	if (!ensure(WeaponMesh)) return;

	HitActors.Reset();
	SetComponentTickEnabled(true);
}

void UBladeWeaponTraceComponent::StopTrace()
{
	SetComponentTickEnabled(false);
}

