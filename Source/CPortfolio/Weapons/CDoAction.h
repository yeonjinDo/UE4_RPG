#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "CDoAction.generated.h"

UCLASS(Abstract, NotBlueprintable)
class CPORTFOLIO_API UCDoAction : public UObject
{
	GENERATED_BODY()


public:
	FORCEINLINE bool GetBeginAction() { return bBeginAction; }
	FORCEINLINE void SetBeginAction(bool b) { bBeginAction = b; }
	FORCEINLINE bool GetInAction() { return bInAction; }


public:
	UCDoAction();

	virtual void BeginPlay
	(
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		class ACharacter* InOwner,
		const TArray<FDoActionData>& InDoActionData,
		const TArray<FHitData>& InHitData
	);

	virtual void Tick(float InDeltaTime) {}


public:
	virtual void DoAction();
	virtual void Begin_DoAction();
	virtual void End_DoAction();

public:
public:
	UFUNCTION()
		virtual void OnBeginEquip() { }

	UFUNCTION()
		virtual void OnUnequip() { }

	UFUNCTION()
		virtual void OnAttachmentBeginCollision() { }

	UFUNCTION()
		virtual void OnAttachmentEndCollision() { }

	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther) { }

	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) { }

protected:
	bool bInAction;
	bool bBeginAction;

	class ACharacter* OwnerCharacter;
	class UWorld* World;	//얘는 UObject를직접 상속받아 World가 없다 Owner에서 받아와서 사용할거다

	class UCMovementComponent* Movement;
	class UCStateComponent* State;


	TArray<FDoActionData> DoActionDatas;	//칼 하나에 동작 세개니까 array로 가야지
	TArray<FHitData> HitDatas;



};
