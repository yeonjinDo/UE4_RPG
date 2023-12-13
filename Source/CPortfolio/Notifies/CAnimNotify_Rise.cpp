#include "Notifies/CAnimNotify_Rise.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Components/CSubStateComponent.h"


FString UCAnimNotify_Rise::GetNotifyName_Implementation() const
{
	return "Rise";
}

void UCAnimNotify_Rise::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCStateComponent* State = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());

	if(!!State)
	{
		State->SetRiseMode();
	}

}
