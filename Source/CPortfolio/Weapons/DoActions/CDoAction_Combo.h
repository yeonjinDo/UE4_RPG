#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Combo.generated.h"

UCLASS()
class CPORTFOLIO_API UCDoAction_Combo : public UCDoAction
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo()  { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

	void Tick(float InDeltaTime) override;


public:
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther) override;
	void OnAttachmentEndCollision() override;


private:
	int32 Index;	//몇번째 콤보인지

	bool bEnable;	//콤보가 가능한 구간인지
	bool bExist;	//콤보가 존재하는지

private:
	TArray<class ACharacter*> Hitted;

	bool HittedRotation = false;
	FRotator TargetRotation;
};

