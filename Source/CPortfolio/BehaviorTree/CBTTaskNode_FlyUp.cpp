#include "BehaviorTree/CBTTaskNode_FlyUp.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CAIController_Monster.h"
#include "Characters/CMonster.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStatusComponent.h"

UCBTTaskNode_FlyUp::UCBTTaskNode_FlyUp()
{
	NodeName = "Fly";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_FlyUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACMonster* monster = Cast<ACMonster>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(monster);

	
	FVector Location;
	Location = monster->GetActorLocation();


	FVector start = monster->GetActorLocation();
	FVector end = start + monster->GetActorUpVector() * (-500);

	TArray<AActor*> ignores;
	ignores.Add(monster);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery2, false, ignores, EDrawDebugTrace::None, hitresult, 
										true, FLinearColor::Green, FLinearColor::Red);

	if (Location.Z >= hitresult.Location.Z + MaxHeight)
		return EBTNodeResult::Succeeded;

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_FlyUp::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACMonster* monster = Cast<ACMonster>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(monster);


	FVector Location;
	Location = monster->GetActorLocation();
	Location.Z += 3.0f;

	monster->SetActorLocation(Location);

	if (Location.Z >= hitresult.Location.Z + MaxHeight)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
