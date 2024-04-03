#include "BehaviorTree/CBTService_Melee.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";		//BehaviorTree�� ��Ÿ�� �̸�

	Interval = 0.1f;		//tick ����
	RandomDeviation = 0.0f; //tick���ݿ��� ������ �ִ°�

}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent*      state    = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	CheckTrue(aiState->IsDeadMode());
	
	if (state->IsHittedMode())
	{
		aiState->SetHittedMode();
	}

	ACharacter* target = aiState->GetTarget();

	if(target == nullptr)
	{
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		aiState->SetPatrolMode();
		return;
	}

	controller->SetFocus(target);

	float distance = ai->GetDistanceTo(target);


	if (distance > ActionRange + ApproachOffset)
	{
		aiState->SetApproachMode();
	}



	if (bGroupFight == true)
	{
		//�� Ÿ�ٿ� ������ �������� ! 
		if (controller->GetBlackboardComponent()->GetValueAsInt("Attacking") >= MaxAttack)
		{
			aiState->SetStrafeMode();
			return;
		}
	}

	CheckTrue(aiState->IsActionMode());
	if (distance < ActionRange)
	{

		aiState->SetActionMode();
		
		return;
	}
	
	aiState->SetApproachMode();

}
