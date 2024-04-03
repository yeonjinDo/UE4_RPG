#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Dash.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTTaskNode_Dash : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Launch")
		float LaunchRate = 5000;

	UPROPERTY(EditAnywhere, Category = "Dash")
		UNiagaraSystem* DashEffect;


public:
	UCBTTaskNode_Dash();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


};
