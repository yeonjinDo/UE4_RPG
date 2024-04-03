#include "Weapons/CSubAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CapsuleComponent.h"


UCSubAction::UCSubAction()
{
}


void UCSubAction::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Owner      = InOwner;
	Attachment = InAttachment;
	DoAction   = InDoAction;

	State      = CHelpers::GetComponent<UCStateComponent>(Owner);
	Movement   = CHelpers::GetComponent<UCMovementComponent>(Owner);

}

void UCSubAction::Pressed()
{	//CAnimInstance -> Bow_Aiming ����ó������ ���
	
	bInAction = true;
}

void UCSubAction::Released()
{
	bInAction = false;
}

float UCSubAction::GetPercent()
{
	return 0;
}
