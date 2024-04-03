#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Boss.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTService_Boss : public UBTService
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Range")
		float ActionRange = 800;

	UPROPERTY(EditAnywhere, Category = "Range")
		float ApproachRange = 1500;

	UPROPERTY(EditAnywhere, Category = "Debug")
		bool bDrawDebug;

	UPROPERTY(EditAnywhere, Category = "Action")
		float AvoidRange = 200;

	UPROPERTY(EditAnywhere, Category = "Action")
		int32 randomMax = 20;


public:
	UCBTService_Boss();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	int32 hittedCnt = 0;
	ACharacter* target;
};
