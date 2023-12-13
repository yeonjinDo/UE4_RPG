#include "Notifies/CAnimNotify_Boss_SpawnFire.h"
#include "Global.h"
#include "Characters/CAIController_Boss.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"

FString UCAnimNotify_Boss_SpawnFire::GetNotifyName_Implementation() const
{
	return "SpawnFire";
}

void UCAnimNotify_Boss_SpawnFire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(MeshComp->GetOwner());
	UCStateComponent* State = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());

	if (!!State)
		CheckTrue(State->IsActionMode());

	FActorSpawnParameters params;
	params.Owner = MeshComp->GetOwner();
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;
	transform.SetLocation(MeshComp->GetOwner()->GetActorLocation());
	transform.SetRotation(FQuat(MeshComp->GetOwner()->GetActorRotation()));


	MeshComp->GetOwner()->GetWorld()->SpawnActor<ACBoss_Ray>(FireClass, transform, params);
	
}
