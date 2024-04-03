#include "Components/CWeaponComponent.h"
#include "Global.h"
#include "CStateComponent.h"
#include "CStatusComponent.h"
#include "CSubStateComponent.h"
#include "GameFramework/Character.h"
#include "Utilities/CHelpers.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CWeaponData.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CSkillAction.h"
#include "Weapons/CSubAction.h"

UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	for (int32 i = 0; i < (int32)EWeaponType::Max; i++)
	{
		if (!!DataAssets[i])
			DataAssets[i]->BeginPlay(OwnerCharacter, &Datas[i]);
	}
}


void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!!GetDoAction())
		GetDoAction()->Tick(DeltaTime);

	if (!!GetSubAction())
	{
		GetSubAction()->Tick(DeltaTime);
	}
}

bool UCWeaponComponent::IsIdleMode()
{
	return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsIdleMode();
}

ACAttachment * UCWeaponComponent::GetAttachment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetAttachment();

}

ACAttachment* UCWeaponComponent::GetSubAttachment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetSubAttachment();
}

UCEquipment * UCWeaponComponent::GetEquipment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetEquipment();
}

UCDoAction* UCWeaponComponent::GetDoAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetDoAction();
}

UCSubAction * UCWeaponComponent::GetSubAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetSubAction();
}

UCSkillAction* UCWeaponComponent::GetSkillAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetSkillAction();
}

void UCWeaponComponent::SetUnarmedMode()
{
	CheckFalse(IsIdleMode());

	GetEquipment()->Unequip();

	PrevType = EWeaponType::Max;

	ChangeType(EWeaponType::Max);
}

void UCWeaponComponent::SetFistMode()
{
	CheckFalse(IsIdleMode());

	PrevType = EWeaponType::Fist;

	SetMode(EWeaponType::Fist);
}

void UCWeaponComponent::SetSwordMode()
{
	CheckFalse(IsIdleMode());

	PrevType = EWeaponType::Sword;

	SetMode(EWeaponType::Sword);
}

void UCWeaponComponent::SetHammerMode()
{
	CheckFalse(IsIdleMode());

	PrevType = EWeaponType::Hammer;

	SetMode(EWeaponType::Hammer);
}

void UCWeaponComponent::SetWarpMode()
{
	CheckFalse(IsIdleMode());

	PrevType = EWeaponType::Warp;

	SetMode(EWeaponType::Warp);
}

void UCWeaponComponent::SetAroundMode()
{
	CheckFalse(IsIdleMode());

	PrevType = EWeaponType::Around;

	SetMode(EWeaponType::Around);
}

void UCWeaponComponent::SetBowMode()
{
	CheckFalse(IsIdleMode());

	PrevType = EWeaponType::Bow;

	SetMode(EWeaponType::Bow);
}

void UCWeaponComponent::SetPrevMode()
{
	CheckFalse(IsIdleMode());

	if (PrevType == EWeaponType::Max)
	{
		SetUnarmedMode();
	}
	else 
		SetMode(PrevType);
}

void UCWeaponComponent::DoAction()
{
	if (!!GetDoAction())
	{
		UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

		if(!!status)
		{
			CheckTrue(status->EmptyMana());
			status->UseMana(2.0f);
		}

		GetDoAction()->DoAction();
	}
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{

	if (Type == InType)
	{
		SetUnarmedMode();
		return;
	}
	else if (IsUnarmedMode() == false)
	{
		GetEquipment()->Unequip();
	}
	

	if (!!Datas[(int32)InType])
	{
		Datas[(int32)InType]->GetEquipment()->Equip();
		ChangeType(InType);
	}

}


void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType prevType = Type;
	Type = InType;

	if (OnWeaponTypeChange.IsBound())
		OnWeaponTypeChange.Broadcast(prevType, InType);

}

void UCWeaponComponent::SubAction_Pressed()
{
	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	if (!!status)
	{
		CheckTrue(status->EmptyMana());

		if (!!GetSubAction())
		{
			status->UseMana(5.0f);
			GetSubAction()->Pressed();
		}
	}
	else
	{
		if (!!GetSubAction())
			GetSubAction()->Pressed();
	}
	
}

void UCWeaponComponent::SubAction_Released()
{
	if (!!GetSubAction())
		GetSubAction()->Released();
}

void UCWeaponComponent::SkillAction_Pressed()
{
	if (!!GetSkillAction())
		GetSkillAction()->Pressed();
}
