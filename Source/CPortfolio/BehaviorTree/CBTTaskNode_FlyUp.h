#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_FlyUp.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTTaskNode_FlyUp : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_FlyUp();


public:
	UPROPERTY(EditAnywhere)
		float MaxHeight = 100;

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	FHitResult hitresult;
	
};
