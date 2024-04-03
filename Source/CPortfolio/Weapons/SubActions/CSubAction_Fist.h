#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CSubAction_Fist.generated.h"


UCLASS(Blueprintable)
class CPORTFOLIO_API UCSubAction_Fist : public UCSubAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Action")
		FDoActionData ActionData;

	UPROPERTY(EditAnywhere, Category = "Action")
		TArray<FHitData> HitDatas;


	UPROPERTY(EditAnywhere, Category = "Add-On")
		TSubclassOf<class ACGhostTrail> GhostTrailClass;

private:
	UCSubAction_Fist();

public:
	void Pressed() override;

	void Begin_SubAction_Implementation() override;
	void End_SubAction_Implementation() override;

private:
	UFUNCTION()
		void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);

	UFUNCTION()
		void OnAttachmentEndCollision();

	UFUNCTION()
		void OnCoolDown();

private:
	TArray<class ACharacter*> Hitted;
	int32 HitIndex;


private:
	class ACGhostTrail* GhostTrail;

	FTimerHandle TimerHandle;
};


