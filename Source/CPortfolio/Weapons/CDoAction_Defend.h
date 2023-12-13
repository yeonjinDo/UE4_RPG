#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Defend.generated.h"

UCLASS()
class CPORTFOLIO_API UCDoAction_Defend : public UCDoAction
{
	GENERATED_BODY()

public:
	UCDoAction_Defend();

	virtual void BeginPlay
	(
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		class ACharacter* InOwner,
		const TArray<FDoActionData>& InDoActionData,
		const TArray<FHitData>& InHitData
	);
	void Tick(float InDeltaTime) override;


public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

private:
	class UCSkillAction_Sword* skill;
};
