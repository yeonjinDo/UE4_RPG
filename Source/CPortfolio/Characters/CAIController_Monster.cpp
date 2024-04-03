#include "Characters/CAIController_Monster.h"

#include "CMonster.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CAIBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ACAIController_Monster::ACAIController_Monster()
{
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");

	//������Ʈ�� �ƴѰ�ü�� �����Ҵ� �� �� CreateDefaultSubobject ���
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");

	Sight->SightRadius = 800;					//�þ� �ݰ�
	Sight->LoseSightRadius = 800;
	Sight->PeripheralVisionAngleDegrees = 55;	//�þ� ����
	Sight->SetMaxAge(2);						//���� �Ҵ°�

	//�� ���� �Ұ��� ( TeamID )
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;


	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());	//�켱����


}

void ACAIController_Monster::BeginPlay()
{
	Super::BeginPlay();

	//�̺�Ʈ ����
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController_Monster::OnPerceptionUpdated);
}

void ACAIController_Monster::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	//�����Ȱ� �������� �� �����ϱ� �迭��
	TArray<AActor*> actors;

	Perception->GetCurrentlyPerceivedActors(nullptr, actors);

	if (actors.Num() > 0)
	{
		Blackboard->SetValueAsObject("Target", actors[0]);

		return;
	}

	Blackboard->SetValueAsObject("Target", nullptr);



}

void ACAIController_Monster::OnPossess(APawn* InPawn)
{	//���� �Ǿ����� ȣ��

	Super::OnPossess(InPawn);

	Monster = Cast<ACMonster>(InPawn);

	//TeamID ����
	SetGenericTeamId(Monster->GetGenericTeamId());

	//Behavior Tree�ȿ� Blackboard �־��ֱ�
	CheckNull(Monster->GetBehaviorTree());
	UseBlackboard(Monster->GetBehaviorTree()->BlackboardAsset, Blackboard);

	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(Monster);
	Behavior->SetBlackboard(Blackboard);

	//

	RunBehaviorTree(Monster->GetBehaviorTree());

}

void ACAIController_Monster::OnUnPossess()
{
	Super::OnUnPossess();
}


