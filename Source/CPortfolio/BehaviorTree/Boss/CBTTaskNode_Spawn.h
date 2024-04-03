#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Weapons/CWeaponStructures.h"
#include "CBTTaskNode_Spawn.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTTaskNode_Spawn : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Fire")
		TSubclassOf<class ACRandomObject> RandomObject;

private:
	UPROPERTY(EditAnywhere, Category = "CameraShake")
		TSubclassOf<class UMatineeCameraShake> CameraShakeClass;

	UPROPERTY(EditAnywhere, Category = "Montage")
		UAnimMontage* Montage;



public:
	UCBTTaskNode_Spawn();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;



private:
	TArray<FVector> RandomPoints;
	TArray<FHitData> HitDatas;
	ACRandomObject* Object;

};
