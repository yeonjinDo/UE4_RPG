#include "BehaviorTree/Boss/CBTTaskNode_GetRadomPoints.h"
#include "Global.h"
#include "NavigationSystem.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Components/CAIBehaviorComponent.h"
#include "Weapons/AddOns/Boss/CNotifyCircle.h"


UCBTTaskNode_GetRadomPoints::UCBTTaskNode_GetRadomPoints()
{
	NodeName = "RandomPoints";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_GetRadomPoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);


	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI_Boss* ai = Cast<ACEnemy_AI_Boss>(controller->GetPawn());
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	controller->StopMovement();

	FVector location = ai->GetActorLocation();
	
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ai->GetWorld());
	CheckNullResult(navSystem, EBTNodeResult::Failed);

	FNavLocation point(location);

	FActorSpawnParameters params;
	params.Owner = ai;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;

	for (int32 i = 0; i < MaxSpawn; i++)
	{
		while (true)
		{
			if (navSystem->GetRandomPointInNavigableRadius(location, RandomRadius, point))
			{
				ai->GetWorld()->SpawnActor<ACNotifyCircle>(Notify, point, transform.Rotator(), params);
			
				break;
			}
		}
		RandomPoints.Add(point.Location);
	}

	aiState->SetRandomPoints(RandomPoints);
	RandomPoints.Empty();

	//몽타주 재생
	ai->PlayAnimMontage(Montage);

	//카메라 셰이크
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(CameraShakeClass);


	return EBTNodeResult::Succeeded;
}
