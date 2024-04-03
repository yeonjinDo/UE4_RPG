#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_MoveForward.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTTaskNode_MoveForward : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_MoveForward();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	FVector TargetLocation;
};
