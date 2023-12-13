#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Boss_Actions.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTService_Boss_Actions : public UBTService
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Phase")
		float PhaseOne = 100;

	UPROPERTY(EditAnywhere, Category = "Phase")
		float PhaseTwo = 60;

	UPROPERTY(EditAnywhere, Category = "Phase")
		float PhaseThree = 30;

public:
	UCBTService_Boss_Actions();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
