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

	//task�� ����ϴ� ��ü(ĳ����, ��)�� �޶� ���� �ּ�(�޸�)�� �����Ѵ�,, ���� �½�ũ�� �����Ѵ�?
	//CLog::Log(FString::Printf(L"%s, %p", *ai->GetActorLabel(), this));

	FVector location = ai->GetActorLocation();

	//����Ǵ� ai�������� ��� nav volume�� �ö� �ִ��� ���Ѵ�
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ai->GetWorld());
	CheckNullResult(navSystem, EBTNodeResult::Failed);

	//navigation���� �̵��� ��ġ�� ���ϴ� ��
	FNavLocation point(location);
	while (true)
	{
		if (navSystem->GetRandomPointInNavigableRadius(location, RandomRadius, point))
			break;			//�� �� ���� ��ġ�� ���� �ÿ� false ��ȯ�Ѵ�. true�� ���ö�����(�� �� �ִ� ��ġ�� ���ö�����) ��������Ѵ�
	}

	//behavior�� ������ ���� �־��ش�
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
				//�����̳� ���з� ������ FinishLatentTask/ �ߴ����� ������ FinishLatentAbort
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
	//����� ���
	if (bDebugMode)
		DrawDebugSphere(InWorld, InLocation, 10, 10, FColor::Green, true, 5);
}
