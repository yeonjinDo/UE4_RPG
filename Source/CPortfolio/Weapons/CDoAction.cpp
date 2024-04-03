#include "Weapons/CDoAction.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CSubStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStatusComponent.h"
#include "Characters/CEnemy.h"


UCDoAction::UCDoAction()
{
}

void UCDoAction::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner,
	const TArray<FDoActionData>& InDoActionData, const TArray<FHitData>& InHitData)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld();

	State    = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);

	DoActionDatas = InDoActionData;
	HitDatas = InHitData;
}

void UCDoAction::DoAction()
{
	CheckTrue(State->IsFrozeMode());	//Froze상태에서는 공격 불가능

	bInAction = true;

	State->SetActionMode();
}

void UCDoAction::Begin_DoAction()
{

	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	CheckTrue(status->IsDead());

	bBeginAction = true;
}

void UCDoAction::End_DoAction()
{
	bInAction = false;
	bBeginAction = false;

	State->SetIdleMode();
	
	Movement->Move();
	Movement->DisableFixedCamera();

}

