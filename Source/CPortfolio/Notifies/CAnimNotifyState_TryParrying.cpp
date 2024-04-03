#include "Notifies/CAnimNotifyState_TryParrying.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CDefendComponent.h"


FString UCAnimNotifyState_TryParrying::GetNotifyName_Implementation() const
{
	return "TryParrying";
}

void UCAnimNotifyState_TryParrying::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);
	player->CanParrying(true);

}

void UCAnimNotifyState_TryParrying::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UCAnimNotifyState_TryParrying::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);
	player->CanParrying(false);

}
