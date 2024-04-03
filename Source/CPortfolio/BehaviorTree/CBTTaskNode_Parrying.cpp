#include "BehaviorTree/CBTTaskNode_Parrying.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Parrying::UCBTTaskNode_Parrying()
{
	bNotifyTick = true;

	NodeName = "Parrying";
}

EBTNodeResult::Type UCBTTaskNode_Parrying::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	controller->StopMovement();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Parrying::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

	if (state->IsParryingMode() == false)	//IsIdleMode로 돌아온 상태 
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		return;
	}

}
