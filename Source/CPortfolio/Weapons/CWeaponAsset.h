#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapons/CWeaponStructures.h"
#include "CWeaponAsset.generated.h"

UCLASS()
class CPORTFOLIO_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)	//어떤 attachment인지 선택을 해주어야하기 때문
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACAttachment> SubAttachmentClass;

	UPROPERTY(EditAnywhere)
		FEquipmentData EquipmentData;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere)
		TArray<FDoActionData> DoActionDatas;

	UPROPERTY(EditAnywhere)
		TArray<FHitData> HitDatas;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCSubAction> SubActionClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCSkillAction> SkillActionClass;


public:
	UCWeaponAsset();

	//원래 BeginPlay는 AActor부터 있다, 얘는 이름만 같게 만든 함수다
	void BeginPlay(class ACharacter* InOwner, class UCWeaponData** OutWeaponData);//생성해서 리턴해줄거기 때문에 2차 포인터	



};
