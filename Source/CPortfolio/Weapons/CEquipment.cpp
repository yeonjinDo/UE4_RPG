#include "Weapons/CEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"


void UCEquipment::BeginPlay(ACharacter* InOwner, const FEquipmentData& InData)
{
	OwnerCharacter = InOwner;
	Data = InData;

	Movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);
	State    = CHelpers::GetComponent<UCStateComponent>(InOwner);

}

void UCEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	if (Data.bCanMove == false)
		Movement->Stop();

	if(!!Data.Montage)
	{
		OwnerCharacter->PlayAnimMontage(Data.Montage, Data.PlayRate);
	}
	else
	{
		Begin_Equip();
		End_Equip();
	}

	if (Data.bUseControlRotation)
		Movement->EnableControlRotation();


}

void UCEquipment::Begin_Equip_Implementation()
{
	bBeginEquip = true;

	if (OnEquipmentBeginEquip.IsBound())
		OnEquipmentBeginEquip.Broadcast();
}

void UCEquipment::End_Equip_Implementation()
{
	bBeginEquip = false;
	bEquipped = true;

	Movement->Move();
	State->SetIdleMode();

	if (OnEquipmentEndEquip.IsBound())
		OnEquipmentEndEquip.Broadcast();

}

void UCEquipment::Unequip_Implementation()
{
	bEquipped = false;
	Movement->DisableControlRotation();

	if (OnEquipmentUnequip.IsBound())
		OnEquipmentUnequip.Broadcast();
}


