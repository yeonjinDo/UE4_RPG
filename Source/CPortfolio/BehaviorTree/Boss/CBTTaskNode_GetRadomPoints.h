#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_GetRadomPoints.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTTaskNode_GetRadomPoints : public UBTTaskNode
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere, Category = "CameraShake")
		TSubclassOf<class UMatineeCameraShake> CameraShakeClass;

	UPROPERTY(EditAnywhere, Category = "Particle")
		TSubclassOf<class ACNotifyCircle> Notify;

	UPROPERTY(EditAnywhere, Category = "Montage")
		UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, Category = "Random")
		float RandomRadius = 1500;		//������ �ݰ�

	UPROPERTY(EditAnywhere, Category = "Random")
		float MaxSpawn = 10;			//������ ����Ʈ ����
		



public:
	UCBTTaskNode_GetRadomPoints();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	TArray<FVector> RandomPoints;
	FTimerHandle TimerHandle;
};
