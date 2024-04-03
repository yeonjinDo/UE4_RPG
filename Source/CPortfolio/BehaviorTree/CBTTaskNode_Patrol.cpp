#include "BehaviorTree/CBTTaskNode_Patrol.h"
#include "Global.h"
#include "CPatrolPath.h"
#include "Components/SplineComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "NavigationSystem.h"
#include "CPatrolPath.h"


UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";

	bNotifyTick = true;

}


EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);


	if(!!ai->GetPatrolPath())
	{
		FVector moveToPoint = ai->GetPatrolPath()->GetMoveTo();
		behavior->SetPatrolLocation(moveToPoint);

		DrawDebug(ai->GetWorld(), moveToPoint);

		return EBTNodeResult::InProgress;

	}

	//task를 사용하는 객체(캐릭터, 적)이 달라도 같은 주소(메모리)를 공유한다,, 같은 태스크를 공유한다?
	//CLog::Log(FString::Printf(L"%s, %p", *ai->GetActorLabel(), this));

	FVector location = ai->GetActorLocation();

	//실행되는 ai기준으로 어느 nav volume에 올라가 있는지 구한다
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ai->GetWorld());
	CheckNullResult(navSystem, EBTNodeResult::Failed);

	//navigation에서 이동할 위치를 구하는 것
	FNavLocation point(location);
	while (true)
	{
		if (navSystem->GetRandomPointInNavigableRadius(location, RandomRadius, point))
			break;			//갈 수 없는 위치가 나올 시에 false 반환한다. true가 나올때까지(갈 수 있는 위치가 나올때까지) 돌려줘야한다
	}

	//behavior에 움직일 곳을 넣어준다
	behavior->SetPatrolLocation(point.Location);

	DrawDebug(ai->GetWorld(), point.Location);

	return EBTNodeResult::InProgress;

}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);


	FVector location = behavior->GetPatrolLocation();
	EPathFollowingRequestResult::Type result = controller->MoveToLocation(location, AcceptanceDistance, false);

	switch (result)
	{
		case EPathFollowingRequestResult::Failed:
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				//성공이나 실패로 끝나면 FinishLatentTask/ 중단으로 끝나면 FinishLatentAbort
		}
		break;

		case EPathFollowingRequestResult::AlreadyAtGoal:
		{
			if (ai->GetPatrolPath())
				ai->GetPatrolPath()->UpdateIndex();

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		break;

	}

}

void UCBTTaskNode_Patrol::DrawDebug(UWorld* InWorld, FVector InLocation)
{
	//디버깅 모드
	if (bDebugMode)
		DrawDebugSphere(InWorld, InLocation, 10, 10, FColor::Green, true, 5);
}
