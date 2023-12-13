#include "BehaviorTree/Boss/CBTTaskNode_Dash.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CMovementComponent.h"


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

	USkeletalMeshComponent* skeletalmesh = CHelpers::GetComponent<USkeletalMeshComponent>(ai);
	
	ai->LaunchCharacter(ai->GetActorForwardVector() * LaunchRate, false, false);

	
	return EBTNodeResult::InProgress;

}

void UCBTTaskNode_Dash::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UCBTTaskNode_Dash::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	return EBTNodeResult::InProgress;
}
