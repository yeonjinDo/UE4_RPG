#include "BehaviorTree/CBTService_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";		//BehaviorTree에 나타날 이름

	Interval = 0.1f;		//tick 간격
	RandomDeviation = 0.0f; //tick간격에서 랜덤값 주는것

}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent*      state    = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	CheckTrue(aiState->IsDeadMode());

	//CLog::Log("Checking");
	//if (state->IsParryingMode())
	//{
	//	CLog::Log("ParryingMode");
	//	aiState->SetParryingMode();
	//	return;
	//}

	////상태 Mode 설정
	//if(state->IsHittedMode())
	//{
	//	CLog::Log("HittedMode");
	//	aiState->SetHittedMode();
	//	return;
	//}


	ACharacter* target = aiState->GetTarget();

	if(target == nullptr)
	{
		aiState->SetPatrolMode();

		return;
	}

	
	UCStateComponent* targetState = CHelpers::GetComponent<UCStateComponent>(target);
	if(!!targetState)
	{
		if (targetState->IsActionMode())
		{
			aiState->IsWaitMode();
			return;
		}
		else
		{
			float distance = ai->GetDistanceTo(target);
			if (distance < ActionRange)
			{
				aiState->SetActionMode();

				return;
			}
		}
	}


	

	aiState->SetApproachMode();


}
