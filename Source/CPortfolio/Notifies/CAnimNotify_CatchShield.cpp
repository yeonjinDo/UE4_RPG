#include "Notifies/CAnimNotify_CatchShield.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/Attachments/CAttachment_Shield.h"


class UCWeaponComponent;

FString UCAnimNotify_CatchShield::GetNotifyName_Implementation() const
{
	return "CatchShield";
}

void UCAnimNotify_CatchShield::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetSubAttachment());

	ACAttachment_Shield* shield = Cast<ACAttachment_Shield>(weapon->GetSubAttachment());
	CheckNull(shield);

	shield->Catch();
}
