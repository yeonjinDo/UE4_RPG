#include "Notifies/CAnimNotify_AvoidAttack.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CMontagesComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapons/CWeaponAsset.h"

FString UCAnimNotify_AvoidAttack::GetNotifyName_Implementation() const
{
	return "AvoidAttack";
}

void UCAnimNotify_AvoidAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	if (player->AvoidAttack())
	{
		UCMontagesComponent* montages = CHelpers::GetComponent<UCMontagesComponent>(player);
		montages->PlayCounterMode();
	}	
}
