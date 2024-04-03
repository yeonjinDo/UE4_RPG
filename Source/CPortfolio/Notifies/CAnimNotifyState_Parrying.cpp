#include "Notifies/CAnimNotifyState_Parrying.h"
#include "Global.h"
#include "Characters/CEnemy_Human.h"
#include "Characters/CPlayer.h"
#include "Components/CDefendComponent.h"


FString UCAnimNotifyState_Parrying::GetNotifyName_Implementation() const
{
	return "Parrying";
}

void UCAnimNotifyState_Parrying::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy_Human* enemy = Cast<ACEnemy_Human>(MeshComp->GetOwner());
	CheckNull(enemy);

	enemy->CanParrying(true);

}

void UCAnimNotifyState_Parrying::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	ACEnemy_Human* enemy = Cast<ACEnemy_Human>(MeshComp->GetOwner());
	CheckNull(enemy);

	enemy->CanParrying(false);

}
