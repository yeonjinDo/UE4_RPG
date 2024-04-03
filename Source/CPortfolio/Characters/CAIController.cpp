#include "Characters/CAIController.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ACAIController::ACAIController()
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
	Sight->DetectionByAffiliation.bDetectEnemies    = true;
	Sight->DetectionByAffiliation.bDetectNeutrals   = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	
	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());	//�켱����
	
	
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	//�̺�Ʈ ����
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	//�����Ȱ� �������� �� �����ϱ� �迭��
	TArray<AActor*> actors;
	
	Perception->GetCurrentlyPerceivedActors( nullptr, actors);

	if(actors.Num() > 0)
	{
		Blackboard->SetValueAsObject("Target", actors[0]);
	
		return;
	}

	Blackboard->SetValueAsObject("Target", nullptr);



}

void ACAIController::OnPossess(APawn* InPawn)
{	//���� �Ǿ����� ȣ��

	Super::OnPossess(InPawn);

	Enemy = Cast<ACEnemy_AI>(InPawn);

	//TeamID ����
	SetGenericTeamId(Enemy->GetGenericTeamId());

	//Behavior Tree�ȿ� Blackboard �־��ֱ�
	CheckNull(Enemy->GetBehaviorTree());
	UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, Blackboard);

	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(Enemy);
	Behavior->SetBlackboard(Blackboard);
	
	//

	RunBehaviorTree(Enemy->GetBehaviorTree());
	
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}


