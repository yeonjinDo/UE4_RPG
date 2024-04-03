#include "Weapons/CWeaponAsset.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoAction.h"
#include "CSubAction.h"
#include "CSkillAction.h"
#include "CWeaponData.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "GameFramework/Character.h"

UCWeaponAsset::UCWeaponAsset()
{
	//Null값이 되지 않도록 기본값을 넣어준다.
	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass  = UCEquipment::StaticClass();

}

void UCWeaponAsset::BeginPlay(ACharacter* InOwner, class UCWeaponData** OutWeaponData)
{
	ACAttachment* Attachment = nullptr;
	if(!!AttachmentClass)
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		//UWeaponAsset은 World에 배치될 애가 아니라서 World가 없다,
		//따라서 InOwner(Player)에서 World를 가져온다
		Attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);

		UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(InOwner);

		if(!!weapon)
		{
			weapon->OnWeaponHidden.AddDynamic(Attachment, &ACAttachment::OnHidden);
			weapon->OnWeaponVisible.AddDynamic(Attachment, &ACAttachment::OnVisible);
		}
		
		ACEnemy_AI* enemy= Cast<ACEnemy_AI>(InOwner);
		if(!!enemy)
		{
			enemy->OnDead.AddDynamic(Attachment, &ACAttachment::OnHidden);
		}
	}

	//방패용 attachment 하나 더 필요
	ACAttachment* SubAttachment = nullptr;
	if (!!SubAttachmentClass)
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		SubAttachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(SubAttachmentClass, params);

		UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(InOwner);

		if (!!weapon)
		{
			weapon->OnWeaponHidden.AddDynamic(SubAttachment, &ACAttachment::OnHidden);
			weapon->OnWeaponVisible.AddDynamic(SubAttachment, &ACAttachment::OnVisible);
		}

		ACEnemy_AI* enemy = Cast<ACEnemy_AI>(InOwner);
		if (!!enemy)
		{
			enemy->OnDead.AddDynamic(SubAttachment, &ACAttachment::OnHidden);
		}

	}


	UCEquipment* Equipment = nullptr;
	if(!!EquipmentClass)
	{
		Equipment = NewObject<UCEquipment>(this, EquipmentClass);
		Equipment->BeginPlay(InOwner, EquipmentData);

	}

	if(!!Attachment)
	{
		Equipment->OnEquipmentBeginEquip.AddDynamic(Attachment, &ACAttachment::OnBeginEquip);
		Equipment->OnEquipmentEndEquip.AddDynamic  (Attachment, &ACAttachment::OnEndEquip);
		Equipment->OnEquipmentUnequip.AddDynamic   (Attachment, &ACAttachment::OnUnequip);
	}

	if (!!SubAttachment)
	{
		Equipment->OnEquipmentBeginEquip.AddDynamic(SubAttachment, &ACAttachment::OnBeginEquip);
		Equipment->OnEquipmentEndEquip.AddDynamic  (SubAttachment, &ACAttachment::OnEndEquip);
		Equipment->OnEquipmentUnequip.AddDynamic   (SubAttachment, &ACAttachment::OnUnequip);
	}

	UCDoAction* DoAction = nullptr;
	if(!!DoActionClass)
	{
		DoAction = NewObject<UCDoAction>(this, DoActionClass);
		DoAction->BeginPlay(Attachment, Equipment, InOwner, DoActionDatas, HitDatas);

		if(!!Attachment)
		{
			Attachment->OnAttachmentBeginCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginCollision);
			Attachment->OnAttachmentEndCollision.AddDynamic  (DoAction, &UCDoAction::OnAttachmentEndCollision);

			Attachment->OnAttachmentBeginOverlap.AddDynamic  (DoAction, &UCDoAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic    (DoAction, &UCDoAction::OnAttachmentEndOverlap);
		}

		if (!!Equipment)
		{
			Equipment->OnEquipmentBeginEquip.AddDynamic(DoAction, &UCDoAction::OnBeginEquip);
			Equipment->OnEquipmentUnequip.AddDynamic(DoAction, &UCDoAction::OnUnequip);
		}

	}

	UCSubAction* SubAction = nullptr;
	if (!!SubActionClass)
	{
		SubAction = NewObject<UCSubAction>(this, SubActionClass);
		SubAction->BeginPlay(InOwner, Attachment, DoAction);
	}

	UCSkillAction* SkillAction = nullptr;
	if (!!SkillActionClass)
	{
		SkillAction = NewObject<UCSkillAction>(this, SkillActionClass);
		SkillAction->BeginPlay(InOwner, SubAttachment, DoAction);
	}

	*OutWeaponData = NewObject<UCWeaponData>();
	(*OutWeaponData)->Attachment = Attachment;
	(*OutWeaponData)->SubAttachment = SubAttachment;
	(*OutWeaponData)->Equipment  = Equipment;
	(*OutWeaponData)->DoAction   = DoAction;
	(*OutWeaponData)->SubAction  = SubAction;
	(*OutWeaponData)->SkillAction  = SkillAction;

}
