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

	//컴포넌트가 아닌객체를 동적할당 할 때 CreateDefaultSubobject 사용
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");

	Sight->SightRadius = 2000;					//시야 반경
	Sight->LoseSightRadius = 2000;
	Sight->PeripheralVisionAngleDegrees = 55;	//시야 각도
	Sight->SetMaxAge(2);						//감지 잃는거

	//적 감지 할건지 ( TeamID )
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());	//우선순위
}

void ACAIController_Boss::BeginPlay()
{
	Super::BeginPlay();

	//이벤트 연결
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController_Boss::OnPerceptionUpdated);

}

void ACAIController_Boss::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{	//감지된게 여러개일 수 있으니까 배열로
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
{		//빙의 되었을때 호출

	Super::OnPossess(InPawn);

	Enemy = Cast<ACEnemy_AI>(InPawn);

	//TeamID 설정
	SetGenericTeamId(FGenericTeamId(Enemy->GetTeamID()));

	//Behavior Tree안에 Blackboard 넣어주기
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
