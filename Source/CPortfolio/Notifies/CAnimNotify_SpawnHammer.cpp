#include "Notifies/CAnimNotify_SpawnHammer.h"
#include "Global.h"
#include "Characters/CEnemy.h"
#include "Weapons/AddOns/Boss/CSpinHammer.h"

FString UCAnimNotify_SpawnHammer::GetNotifyName_Implementation() const
{
	return "SpawnHammer";
}

void UCAnimNotify_SpawnHammer::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy* enemy = Cast<ACEnemy>(MeshComp->GetOwner());
	CheckNull(enemy);


	//Hammer Spawn
	FActorSpawnParameters params;
	params.Owner = MeshComp->GetOwner();
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;
	transform.SetLocation(MeshComp->GetSocketTransform("Hand_Hammer").GetLocation());
	transform.SetRotation(FQuat(MeshComp->GetOwner()->GetActorRotation()));

	enemy->GetWorld()->SpawnActor<ACSpinHammer>(SpinHammerClass, transform, params);

}
