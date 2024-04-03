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

	//컴포넌트가 아닌객체를 동적할당 할 때 CreateDefaultSubobject 사용
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");

	Sight->SightRadius = 800;					//시야 반경
	Sight->LoseSightRadius = 800;
	Sight->PeripheralVisionAngleDegrees = 55;	//시야 각도
	Sight->SetMaxAge(2);						//감지 잃는거

	//적 감지 할건지 ( TeamID )
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;


	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());	//우선순위


}

void ACAIController_Monster::BeginPlay()
{
	Super::BeginPlay();

	//이벤트 연결
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController_Monster::OnPerceptionUpdated);
}

void ACAIController_Monster::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	//감지된게 여러개일 수 있으니까 배열로
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
{	//빙의 되었을때 호출

	Super::OnPossess(InPawn);

	Monster = Cast<ACMonster>(InPawn);

	//TeamID 설정
	SetGenericTeamId(Monster->GetGenericTeamId());

	//Behavior Tree안에 Blackboard 넣어주기
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


