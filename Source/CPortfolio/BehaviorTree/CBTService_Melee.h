#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Melee.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTService_Melee : public UBTService
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Action")
		float ActionRange = 150;

	UPROPERTY(EditAnywhere, Category = "Action")
		bool bGroupFight = false;

	UPROPERTY(EditAnywhere, Category = "Action")
		int MaxAttack = 3;

public:
	UCBTService_Melee();


protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	float ApproachOffset = 100;
};
