#include "BehaviorTree/CBTTaskNode_MoveForward.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CAIController_Monster.h"
#include "Characters/CMonster.h"


UCBTTaskNode_MoveForward::UCBTTaskNode_MoveForward()
{
	NodeName = "MoveForward";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_MoveForward::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController_Monster* controller = Cast<ACAIController_Monster>(OwnerComp.GetOwner());
	ACMonster* monster = Cast<ACMonster>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(monster);

	ACharacter* target = Cast<ACharacter>(controller->GetBlackboardComponent()->GetValueAsObject("Target"));

	CLog::Log(target);

	if (!!target)
	{
		FVector location = target->GetActorLocation() - monster->GetActorLocation();
		TargetLocation = FVector(location.X, location.Y, monster->GetActorLocation().Z);
		
	}


	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_MoveForward::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

}
