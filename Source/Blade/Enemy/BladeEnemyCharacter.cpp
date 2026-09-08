#include "BladeEnemyCharacter.h"

#include "AIController.h"

ABladeEnemyCharacter::ABladeEnemyCharacter()
{

}

AActor* ABladeEnemyCharacter::GetCombatTarget() const
{
	AAIController* AI = Cast<AAIController>(GetController());
	
	return AI ? AI->GetFocusActor() : nullptr;
}
