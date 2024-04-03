#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Jump.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTTaskNode_Jump : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Montage")
		UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, Category = "Launch")
		float LaunchRate = 100;

	UPROPERTY(EditAnywhere, Category = "Debug")
		bool Debug = false;


public:
	UCBTTaskNode_Jump();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	FVector Velocity;
	float ExpectTime = 0.5f;
};
