#include "BehaviorTree/Boss/CBTService_Boss.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Characters/CPlayer.h"
#include "Components/CMovementComponent.h"
#include "Components/CStatusComponent.h"


UCBTService_Boss::UCBTService_Boss()
{
	NodeName = "Boss";		//BehaviorTree에 나타날 이름
	 
	Interval = 0.1f;		//tick 간격
	RandomDeviation = 0.0f; //tick간격에서 랜덤값 주는것
}

void UCBTService_Boss::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI_Boss* ai = Cast<ACEnemy_AI_Boss>(controller->GetPawn());

	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	UCStateComponent*        state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCStatusComponent*      status = CHelpers::GetComponent<UCStatusComponent>(ai);

	
	if (bDrawDebug)
	{
		FVector start = ai->GetActorLocation();
		start.Z -= 25;


		FVector end = ai->GetActorLocation();
		end.Z += 25;

		DrawDebugCylinder(ai->GetWorld(), start, end, ActionRange, 10, FColor::Red, false, Interval);
		DrawDebugCylinder(ai->GetWorld(), start, end, ApproachRange, 10, FColor::Yellow, false, Interval);

	}

	if (status->GetHealth() <= 0)
	{
		aiState->SetDeadMode();
		return;
	}

	CLog::Log((int32)aiState->GetActionType());

	//aiState->SetIdle();

	if(hittedCnt >= 2)
	{
		aiState->SetAvoidMode();
		hittedCnt = 0;

		return;
	}

	//Hitted
	if (state->IsHittedMode())
	{
		aiState->SetHittedMode();

		hittedCnt++;

		return;
	}

	target = aiState->GetTarget();

	//Patrol
	if (target == nullptr)
	{


		aiState->SetPatrolMode();

		return;
	}


	
	float distance = ai->GetDistanceTo(target);


	//Avoid
	int32 random = UKismetMathLibrary::RandomInteger(randomMax);
	
	if (random == 1)
	{
		if (distance < AvoidRange)
		{
			aiState->SetAvoidMode();
		}
		return;
	}


	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(ai);
	//Action
	if (distance < ActionRange)		//  distance < action
	{
		UCStateComponent* targetState = Cast<UCStateComponent>(target);

		movement->Stop();
		
		if (!!targetState)
			CheckTrue(targetState->IsHittedMode())

		aiState->SetAction();

		aiState->SetActionMode();
		return;
	}

	movement->CanMove();

	//Approach
	if (distance >= ActionRange && distance < ApproachRange) //  action < distance < approach
	{
		aiState->SetApproachMode();

		return;
	}

	
	aiState->SetWaitMode();
	aiState->SetIdle();
}
