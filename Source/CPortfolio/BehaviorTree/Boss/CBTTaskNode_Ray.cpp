#include "BehaviorTree/Boss/CBTTaskNode_Ray.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Components/CAIBehaviorComponent.h"
#include "Weapons/AddOns/Boss/CBoss_Ray.h"


UCBTTaskNode_Ray::UCBTTaskNode_Ray()
{
	NodeName = "SpawnRay";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Ray::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);


	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI_Boss* ai = Cast<ACEnemy_AI_Boss>(controller->GetPawn());
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	controller->StopMovement();

	NotifyTransform = aiState->GetNotifyTransform();

	FActorSpawnParameters params;
	(ai->GetWorld()->SpawnActor<ACBoss_Ray>(Ray, NotifyTransform, params))->SetOwner(ai);

	
	return EBTNodeResult::Succeeded;

}

void UCBTTaskNode_Ray::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI_Boss* ai = Cast<ACEnemy_AI_Boss>(controller->GetPawn());
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	if (aiState->IsIdle() || ai->GetEndAction() == true)
	{
		ai->SetEndAction(false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UCBTTaskNode_Ray::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI_Boss* ai = Cast<ACEnemy_AI_Boss>(controller->GetPawn());
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	if (aiState->IsHittedMode())
	{
		aiState->SetIdle();
		return EBTNodeResult::InProgress;
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
