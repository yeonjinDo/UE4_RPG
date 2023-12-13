#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Around.generated.h"

UCLASS(Blueprintable)
class CPORTFOLIO_API UCDoAction_Around : public UCDoAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "SpawnClass")
		TArray<TSubclassOf<class ACRotate_Object>> RotateClasses;

	UPROPERTY(EditAnywhere, Category = "SpawnClass")
		TSubclassOf<class ACShield_Aura> ShieldClass;

public:
	void DoAction() override;
	void Begin_DoAction() override;

private:
	bool SpawnShield = false;
};
