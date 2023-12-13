#include "Notifies/CAnimNotify_EndState.h"
#include "Global.h"
#include "Characters/ICharacter.h"
#include "Components/CDefendComponent.h"

FString UCAnimNotify_EndState::GetNotifyName_Implementation() const
{
	return "EndState";
}

void UCAnimNotify_EndState::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCDefendComponent* defend = CHelpers::GetComponent<UCDefendComponent>(MeshComp->GetOwner());
	CheckNull(defend);
	

	switch (StateType)
	{
		case EStateType::BackStep: character->End_Roll();	break;
		case EStateType::Defend:   defend->End_Defend();	    break;
		case EStateType::Hitted:   character->End_Hitted();	    break;
		case EStateType::Dead:     character->End_Dead();	    break;
	}
}