#include "BehaviorTree/Boss/CBTTaskNode_Spawn.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Characters/CPlayer.h"
#include "Components/CAIBehaviorComponent.h"
#include "Weapons/AddOns/Boss/CRandomObject.h"


UCBTTaskNode_Spawn::UCBTTaskNode_Spawn()
{
	NodeName = "Spawn";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Spawn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	
	controller->StopMovement();

	FActorSpawnParameters params;
	FTransform transform;

	RandomPoints = aiState->GetRandomPoints();
	
	for (int32 i = 0; i < RandomPoints.Num(); i++)
	{

		transform.SetLocation(ai->GetActorLocation());
		
		Object = ai->GetWorld()->SpawnActor<ACRandomObject>(RandomObject, transform, params);
		Object->SetAttacker(ai);
		Object->Init(ai->GetActorLocation(), RandomPoints[i]);
		
	}

	RandomPoints.Empty();
	aiState->GetRandomPoints().Empty();

	
	ai->PlayAnimMontage(Montage);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(CameraShakeClass);

	return EBTNodeResult::Succeeded;
}

void UCBTTaskNode_Spawn::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI_Boss* ai = Cast<ACEnemy_AI_Boss>(controller->GetPawn());
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	if (aiState->IsIdle() || ai->GetEndAction() == true)
	{
		ai->SetEndAction(false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}


}

EBTNodeResult::Type UCBTTaskNode_Spawn::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Succeeded;
}
