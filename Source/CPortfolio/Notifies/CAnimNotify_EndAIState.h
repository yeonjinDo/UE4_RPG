#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Components/CAIBehaviorComponent.h"
#include "CAnimNotify_EndAIState.generated.h"

UCLASS()
class CPORTFOLIO_API UCAnimNotify_EndAIState : public UAnimNotify
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere, Category = "Type")
		EBossActionType BossStateType;

public:
	FString GetNotifyName_Implementation() const override;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;


};
