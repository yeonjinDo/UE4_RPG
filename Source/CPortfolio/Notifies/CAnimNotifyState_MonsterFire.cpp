#include "Notifies/CAnimNotifyState_MonsterFire.h"
#include "Global.h"
#include "Weapons/AddOns/CFire.h"


FString UCAnimNotifyState_MonsterFire::GetNotifyName_Implementation() const
{
	return "Monster Fire";
}

void UCAnimNotifyState_MonsterFire::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	FActorSpawnParameters params;
	params.Owner = MeshComp->GetOwner();
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;
	transform.SetLocation(MeshComp->GetSocketTransform("Mouth").GetLocation());
	transform.SetRotation(FQuat(MeshComp->GetSocketTransform("Mouth").GetRotation()));
	
	MeshComp->GetOwner()->GetWorld()->SpawnActor<ACFire>(FireClass, transform, params);
	
}

void UCAnimNotifyState_MonsterFire::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	//MeshComp->GetOwner()->GetWorld()->DestroyActor(Fire);


	
}
