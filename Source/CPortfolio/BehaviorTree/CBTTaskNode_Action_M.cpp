#include "BehaviorTree/CBTTaskNode_Action_M.h"

UCBTTaskNode_Action_M::UCBTTaskNode_Action_M()
{
}

EBTNodeResult::Type UCBTTaskNode_Action_M::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UCBTTaskNode_Action_M::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UCBTTaskNode_Action_M::AbortTask(UBehaviorTreeComponent& Owner0Comp, uint8* NodeMemory)
{
	return Super::AbortTask(Owner0Comp, NodeMemory);
}
