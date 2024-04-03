#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_End_Parkour.generated.h"

UCLASS()
class CPORTFOLIO_API UCAnimNotify_End_Parkour : public UAnimNotify
{
	GENERATED_BODY()


public:
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;


};
