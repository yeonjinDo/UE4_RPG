#include "Notifies/CAnimNotify_EndAIState.h"
#include "Global.h"
#include "Characters/ICharacter.h"


FString UCAnimNotify_EndAIState::GetNotifyName_Implementation() const
{
	return "EndAIState";
}

void UCAnimNotify_EndAIState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);


	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);

	

	switch (AIStateType)
	{
	case EAIStateType::Action: character->End_AIAction();	break;
	
	}

}
