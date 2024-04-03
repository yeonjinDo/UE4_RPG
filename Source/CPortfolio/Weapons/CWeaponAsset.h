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
	UPROPERTY(EditAnywhere)	//� attachment���� ������ ���־���ϱ� ����
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

	//���� BeginPlay�� AActor���� �ִ�, ��� �̸��� ���� ���� �Լ���
	void BeginPlay(class ACharacter* InOwner, class UCWeaponData** OutWeaponData);//�����ؼ� �������ٰű� ������ 2�� ������	



};
