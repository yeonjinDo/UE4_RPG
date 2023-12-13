#include "Notifies/CAnimNotifyState_SkillAction.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CSkillAction.h"

FString UCAnimNotifyState_SkillAction::GetNotifyName_Implementation() const
{
	return "SkillAction";
}

void UCAnimNotifyState_SkillAction::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetSkillAction());

	weapon->GetSkillAction()->Begin_SkillAction();
}

void UCAnimNotifyState_SkillAction::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetSkillAction());

	weapon->GetSkillAction()->End_SkillAction();

}
