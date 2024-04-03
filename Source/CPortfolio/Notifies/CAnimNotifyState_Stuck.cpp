#include "Notifies/CAnimNotifyState_Stuck.h"
#include "Global.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CSubStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CAttachment.h"


FString UCAnimNotifyState_Stuck::GetNotifyName_Implementation() const
{
	return "Stuck";
}

void UCAnimNotifyState_Stuck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetAttachment());

	CheckTrue(weapon->GetAttachment()->HittedCharacters().Num() <= 0);

	CheckNull(weapon->GetAttachment()->HittedCharacters()[0]);

	Hitted = weapon->GetAttachment()->HittedCharacters()[0];

	
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(Hitted);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(Hitted);
	CheckNull(movement);
	CheckNull(state);
	movement->SetSpeed(ESpeedType::Frozen);
	state->SetFrozeMode();

}

void UCAnimNotifyState_Stuck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(Hitted);

	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(Hitted);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(Hitted);
	CheckNull(movement);
	movement->SetSpeed(ESpeedType::Run);
	state->SetIdleMode();


}
