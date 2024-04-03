#include "BehaviorTree/Boss/CBTTaskNode_Jump.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CMovementComponent.h"

UCBTTaskNode_Jump::UCBTTaskNode_Jump()
{
	NodeName = "Jump";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Jump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI_Boss* ai = Cast<ACEnemy_AI_Boss>(controller->GetPawn());
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(ai);

	controller->StopMovement();

	//타겟의 위치로 점프하며 launch
	FVector startPos = ai->GetActorLocation();
	FVector targetPos = movement->GetFutureLocation(aiState->GetTarget(), ExpectTime);
	FVector endPos = FVector(targetPos.X, targetPos.Y, targetPos.Z + 100);

	FVector direction = endPos - startPos;

	if(Debug)
		DrawDebugSphere(GetWorld(), endPos, 100, 12, FColor::Blue, false, 1, 0, 1);

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), Velocity, startPos, endPos);

	ai->SetActorRotation(direction.ToOrientationRotator());
	ai->LaunchCharacter(Velocity, true, true);


	ai->PlayAnimMontage(Montage);

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Jump::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

EBTNodeResult::Type UCBTTaskNode_Jump::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI_Boss* ai = Cast<ACEnemy_AI_Boss>(controller->GetPawn());
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(ai);

	
	if (aiState->IsHittedMode())
	{
		aiState->SetIdle();
		return EBTNodeResult::InProgress;
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
