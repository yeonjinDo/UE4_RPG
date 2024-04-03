#include "BehaviorTree/Boss/BTTaskNode_AfterDash.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Components/CAIBehaviorComponent.h"
#include "Weapons/AddOns/Boss/CNotifyBox.h"

UBTTaskNode_AfterDash::UBTTaskNode_AfterDash()
{
	NodeName = "Attack";

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_AfterDash::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);


	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI_Boss* ai = Cast<ACEnemy_AI_Boss>(controller->GetPawn());
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	FActorSpawnParameters params;
	params.Owner = ai;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector direction = aiState->GetTarget()->GetActorLocation() - ai->GetActorLocation();
	ai->SetActorRotation(direction.ToOrientationQuat());

	FTransform transform;
	transform.SetLocation(ai->GetActorLocation());
	transform.SetRotation(direction.ToOrientationQuat());

//	ai->GetWorld()->SpawnActor<ACNotifyBox>(Route, transform, params);


	ai->PlayAnimMontage(Montage);


	return EBTNodeResult::InProgress;

}

void UBTTaskNode_AfterDash::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

EBTNodeResult::Type UBTTaskNode_AfterDash::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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