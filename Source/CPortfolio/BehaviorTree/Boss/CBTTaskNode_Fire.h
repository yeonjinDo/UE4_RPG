#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Fire.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTTaskNode_Fire : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Fire")
		TSubclassOf<class ACNotifyBox> Route;

private:
	UPROPERTY(EditAnywhere, Category = "CameraShake")
		TSubclassOf<class UMatineeCameraShake> CameraShakeClass;


	UPROPERTY(EditAnywhere, Category = "Montage")
		UAnimMontage* Montage;

		

public:
	UCBTTaskNode_Fire();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;



private:
	FTimerHandle TimerHandle;
	class ACNotifyBox* NotifyBox;
	float DelayTime = 1;
	ACharacter* Target;
};
