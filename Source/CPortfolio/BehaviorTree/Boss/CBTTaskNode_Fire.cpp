#include "BehaviorTree/Boss/CBTTaskNode_Fire.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Characters/CAIController_Boss.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Components/CAIBehaviorComponent.h"
#include "Weapons/AddOns/Boss/CNotifyBox.h"
#include "Weapons/AddOns/Boss/CBoss_Ray.h"


UCBTTaskNode_Fire::UCBTTaskNode_Fire()
{
	NodeName = "Notify";
		
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI_Boss* ai = Cast<ACEnemy_AI_Boss>(controller->GetPawn());
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	controller->StopMovement();


	FActorSpawnParameters params;
	params.Owner = ai;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector direction = aiState->GetTarget()->GetActorLocation() - ai->GetActorLocation();
	FTransform transform;
	transform.SetLocation(ai->GetActorLocation());
	transform.SetRotation(direction.ToOrientationQuat());
	
	//예상 경로 빨간불 스폰
	NotifyBox = ai->GetWorld()->SpawnActor<ACNotifyBox>(Route, transform, params);

	aiState->SetNotifyTransform(transform);

	ai->SetActorRotation(direction.ToOrientationQuat());
	ai->PlayAnimMontage(Montage);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(CameraShakeClass);

	return EBTNodeResult::InProgress;

}

void UCBTTaskNode_Fire::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

EBTNodeResult::Type UCBTTaskNode_Fire::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI_Boss* ai = Cast<ACEnemy_AI_Boss>(controller->GetPawn());
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);


	if (aiState->IsHittedMode())
	{
		aiState->SetIdle();
		return EBTNodeResult::InProgress;
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
