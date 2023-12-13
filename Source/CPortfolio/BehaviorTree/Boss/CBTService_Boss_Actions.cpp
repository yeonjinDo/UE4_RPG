#include "BehaviorTree/Boss/CBTService_Boss_Actions.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CStatusComponent.h"


UCBTService_Boss_Actions::UCBTService_Boss_Actions()
{
	NodeName = "Boss_Action";		//BehaviorTree에 나타날 이름

	Interval = 0.1f;		//tick 간격
	RandomDeviation = 0.0f; //tick간격에서 랜덤값 주는것
}

void UCBTService_Boss_Actions::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStatusComponent*      status = CHelpers::GetComponent<UCStatusComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	if(status->GetHealth() > PhaseTwo && status->GetHealth() <= PhaseOne)
	{
		aiState->SetPhaseOne();
		return;
	}
	
	if (status->GetHealth() > PhaseThree && status->GetHealth() <= PhaseTwo)
	{
		aiState->SetHitted();
		return;
	}

	if (status->GetHealth() > 0 && status->GetHealth() <= PhaseThree)
	{
		aiState->SetPhaseThree();
		return;
	}
	

}
