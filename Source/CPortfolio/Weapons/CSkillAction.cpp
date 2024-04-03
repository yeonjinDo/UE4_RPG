#include "Weapons/CSkillAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"


UCSkillAction::UCSkillAction()
{
}


void UCSkillAction::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Owner      = InOwner;
	Attachment = InAttachment;
	DoAction   = InDoAction;

	State      = CHelpers::GetComponent<UCStateComponent>(Owner);
	Movement   = CHelpers::GetComponent<UCMovementComponent>(Owner);
}

void UCSkillAction::Pressed()
{	
	bInAction = true;
}

void UCSkillAction::Released()
{
	bInAction = false;
}

float UCSkillAction::GetPercent()
{
	return CurrentTime / CoolTime;
}