#include "BehaviorTree/CBTService_Bettle.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Characters/CAIController.h"
#include "Characters/CAIController_Monster.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CMonster.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CMontagesComponent.h"

UCBTService_Bettle::UCBTService_Bettle()
{
	NodeName = "Monster";		//BehaviorTree에 나타날 이름

	Interval = 0.1f;		//tick 간격
	RandomDeviation = 0.0f; //tick간격에서 랜덤값 주는것

}

void UCBTService_Bettle::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACMonster* monster = Cast<ACMonster>(controller->GetPawn());

	UCStateComponent*      state = CHelpers::GetComponent<UCStateComponent>(monster);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(monster);
	UCMontagesComponent* montage = CHelpers::GetComponent<UCMontagesComponent>(monster);

	CheckTrue(aiState->IsDeadMode());


	ACharacter* target = aiState->GetTarget();


	if (target == nullptr)
	{
		aiState->SetWaitMode();

		return;
	}


	float distance = monster->GetDistanceTo(target);

	

	if (distance < ActionRange)
	{
		aiState->SetActionMode();

		return;
	}


	aiState->SetApproachMode();


}
