#include "BehaviorTree/CBTService_Nerd.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

UCBTService_Nerd::UCBTService_Nerd()
{
	NodeName = "Nerd";		//BehaviorTree�� ��Ÿ�� �̸�

	Interval = 0.1f;		//tick ����
	RandomDeviation = 0.0f; //tick���ݿ��� ������ �ִ°�

}

void UCBTService_Nerd::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent*      state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	CheckTrue(aiState->IsDeadMode());

	//���� Mode ����
	if (state->IsHittedMode())
	{
		aiState->SetHittedMode();
		return;
	}
	if (state->IsParryingMode())
	{
		aiState->SetParryingMode();
		return;
	}


	ACharacter* target = aiState->GetTarget();

	if (target == nullptr)
	{
		aiState->SetPatrolMode();

		return;
	}

	float distance = ai->GetDistanceTo(target);
	if (distance < ActionRange)
	{
		UCSubStateComponent* targetState = Cast<UCSubStateComponent>(target);
		if (!!targetState)
			CheckTrue(targetState->IsFaintMode())

			aiState->SetActionMode();

		return;
	}

	aiState->SetApproachMode();


}

