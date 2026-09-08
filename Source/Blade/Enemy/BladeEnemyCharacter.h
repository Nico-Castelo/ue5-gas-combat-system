#pragma once

#include "CoreMinimal.h"
#include "Core/BladeCharacterBase.h"
#include "BladeEnemyCharacter.generated.h"

UCLASS()
class BLADE_API ABladeEnemyCharacter : public ABladeCharacterBase
{
	GENERATED_BODY()

public:

	ABladeEnemyCharacter();

protected:

	virtual AActor* GetCombatTarget() const override;
};
