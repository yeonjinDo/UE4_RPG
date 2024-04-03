#include "Characters/CAIController_Boss.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ACAIController_Boss::ACAIController_Boss()
{
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");

	//������Ʈ�� �ƴѰ�ü�� �����Ҵ� �� �� CreateDefaultSubobject ���
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");

	Sight->SightRadius = 2000;					//�þ� �ݰ�
	Sight->LoseSightRadius = 2000;
	Sight->PeripheralVisionAngleDegrees = 55;	//�þ� ����
	Sight->SetMaxAge(2);						//���� �Ҵ°�

	//�� ���� �Ұ��� ( TeamID )
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());	//�켱����
}

void ACAIController_Boss::BeginPlay()
{
	Super::BeginPlay();

	//�̺�Ʈ ����
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController_Boss::OnPerceptionUpdated);

}

void ACAIController_Boss::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{	//�����Ȱ� �������� �� �����ϱ� �迭��
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);

	if (actors.Num() > 0)
	{
		Blackboard->SetValueAsObject("Target", actors[0]);

		return;
	}

	Blackboard->SetValueAsObject("Target", nullptr);
}

void ACAIController_Boss::OnPossess(APawn* InPawn)
{		//���� �Ǿ����� ȣ��

	Super::OnPossess(InPawn);

	Enemy = Cast<ACEnemy_AI>(InPawn);

	//TeamID ����
	SetGenericTeamId(FGenericTeamId(Enemy->GetTeamID()));

	//Behavior Tree�ȿ� Blackboard �־��ֱ�
	CheckNull(Enemy->GetBehaviorTree());
	UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, Blackboard);

	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(Enemy);
	Behavior->SetBlackboard(Blackboard);
	//

	RunBehaviorTree(Enemy->GetBehaviorTree());
}

void ACAIController_Boss::OnUnPossess()
{
	Super::OnUnPossess();
}
