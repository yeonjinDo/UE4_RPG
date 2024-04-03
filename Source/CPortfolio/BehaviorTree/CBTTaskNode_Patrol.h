#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Patrol.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTTaskNode_Patrol : public UBTTaskNode
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere, Category = "Patrol")
		bool bDebugMode;

	UPROPERTY(EditAnywhere, Category = "Patrol")
		float AcceptanceDistance = 20;	

	UPROPERTY(EditAnywhere, Category = "Random")
		float RandomRadius = 1500;		//움직일 반경

public:
	UCBTTaskNode_Patrol();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;



private:
	void DrawDebug(UWorld* InWorld, FVector InLocation);



};

