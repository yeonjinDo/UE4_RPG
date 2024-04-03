#include "Notifies/CAnimNotify_EndAIState.h"
#include "Global.h"
#include "Characters/CEnemy_AI_Boss.h"


FString UCAnimNotify_EndAIState::GetNotifyName_Implementation() const
{
	return "EndAIState";
}

void UCAnimNotify_EndAIState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);


	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy_AI_Boss* boss = Cast<ACEnemy_AI_Boss>(MeshComp->GetOwner());
	CheckNull(boss);

	
	switch (BossStateType)
	{
	case EBossActionType::Action: boss->End_Action();	break;
	case EBossActionType::Dash:   boss->End_Dash();	break;
	}

}
