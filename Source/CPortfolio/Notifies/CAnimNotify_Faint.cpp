#include "Notifies/CAnimNotify_Faint.h"
#include "Global.h"
#include "Components/CSubStateComponent.h"


FString UCAnimNotify_Faint::GetNotifyName_Implementation() const
{
	return "Faint";
}

void UCAnimNotify_Faint::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCSubStateComponent* State = CHelpers::GetComponent<UCSubStateComponent>(MeshComp->GetOwner());

	if (!!State)
		State->SetFaintMode();

}
