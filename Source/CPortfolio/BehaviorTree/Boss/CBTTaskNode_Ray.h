#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Ray.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTTaskNode_Ray : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Ray")
		TSubclassOf<class ACBoss_Ray> Ray;

	UPROPERTY(EditAnywhere, Category = "Montage")
		UAnimMontage* Montage;

public:
	UCBTTaskNode_Ray();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


private:
	FTransform NotifyTransform;

};
