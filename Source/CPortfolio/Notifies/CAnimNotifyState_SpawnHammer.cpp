#include "Notifies/CAnimNotifyState_SpawnHammer.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CAIController_Boss.h"
#include "Characters/CEnemy.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/AddOns/Boss/CSpinHammer.h"


FString UCAnimNotifyState_SpawnHammer::GetNotifyName_Implementation() const
{
	return "SpawnSpinningHammer";
}

void UCAnimNotifyState_SpawnHammer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	
	//Origin Weapon Hammer Visibility false;
	ACEnemy* enemy = Cast<ACEnemy>(MeshComp->GetOwner());
	CheckNull(enemy);
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(enemy);
	CheckNull(weapon);
	weapon->GetAttachment()->OnHidden();


	//Hammer Spawn
	FActorSpawnParameters params;
	params.Owner = MeshComp->GetOwner();
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;
	transform.SetLocation(MeshComp->GetSocketTransform("Hand_Hammer").GetLocation());
	transform.SetRotation(FQuat(MeshComp->GetOwner()->GetActorRotation()));

	SpinHammer = enemy->GetWorld()->SpawnActor<ACSpinHammer>(SpinHammerClass, transform, params);

}

void UCAnimNotifyState_SpawnHammer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	//Origin Weapon Hammer Visibility true;
	ACEnemy* enemy = Cast<ACEnemy>(MeshComp->GetOwner());
	CheckNull(enemy);


	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(enemy);
	CheckNull(weapon);
	weapon->GetAttachment()->OnVisible();




}
