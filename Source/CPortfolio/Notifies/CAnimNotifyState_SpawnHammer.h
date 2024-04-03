#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Weapons/AddOns/Boss/CSpinHammer.h"
#include "CAnimNotifyState_SpawnHammer.generated.h"

UCLASS()
class CPORTFOLIO_API UCAnimNotifyState_SpawnHammer : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACSpinHammer> SpinHammerClass;


public:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

private:
	ACSpinHammer* SpinHammer;

};
