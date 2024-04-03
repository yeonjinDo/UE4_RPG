#include "BehaviorTree/Boss/CBTTaskNode_Dash.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


UCBTTaskNode_Dash::UCBTTaskNode_Dash()
{
	NodeName = "Dash";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Dash::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(ai);
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);

	//Movement
	ai->GetCharacterMovement()->GroundFriction = 10;
	ai->GetCharacterMovement()->BrakingDecelerationWalking = 0;
	ai->GetCharacterMovement()->MaxAcceleration = 5000;
	movement->OnDash(DashEffect);

	if (weapon->OnWeaponHidden.IsBound())
		weapon->OnWeaponHidden.Broadcast();

	return EBTNodeResult::Succeeded;

}

void UCBTTaskNode_Dash::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UCBTTaskNode_Dash::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);


	return EBTNodeResult::Succeeded;
}
