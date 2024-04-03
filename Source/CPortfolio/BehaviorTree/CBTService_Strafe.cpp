#include "BehaviorTree/CBTService_Strafe.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStateComponent.h"

UCBTService_Strafe::UCBTService_Strafe()
{
	NodeName = "Strafe";		//BehaviorTree에 나타날 이름

	Interval = 0.1f;		//tick 간격
	RandomDeviation = 0.0f; //tick간격에서 랜덤값 주는것
}

void UCBTService_Strafe::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent*      state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	CheckTrue(aiState->IsDeadMode());


	ACharacter* target = aiState->GetTarget();


	if (target == nullptr)
	{
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		aiState->SetPatrolMode();
		return;
	}


	controller->SetFocus(target);

	float distance = ai->GetDistanceTo(target);

	if (distance > ActionRange)
		aiState->SetApproachMode();


	CheckTrue(aiState->IsActionMode());

	if (distance < ActionRange)
	{
		aiState->SetActionMode();

		return;
	}

	aiState->SetApproachMode();

}
