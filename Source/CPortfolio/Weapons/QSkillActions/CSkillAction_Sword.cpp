#include "Weapons/QSkillActions/CSkillAction_Sword.h"
#include "Global.h"
#include "Weapons/Attachments/CAttachment_Shield.h"
#include "Weapons/AddOns/CShield.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapons/CDoAction.h"
#include "Kismet/KismetMathLibrary.h"

UCSkillAction_Sword::UCSkillAction_Sword()
{

	
}

void UCSkillAction_Sword::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	shield = Cast<ACAttachment_Shield>(InAttachment);
	
}

void UCSkillAction_Sword::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);


}

void UCSkillAction_Sword::Pressed()
{
	Super::Pressed();

	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSubActionMode());

	Super::Pressed();


	State->SetActionMode();
	State->OnSubActionMode();

	ActionData.DoAction(Owner);

	SkillNum = 6;
}

void UCSkillAction_Sword::Begin_SkillAction_Implementation() 
{
	Super::Begin_SkillAction_Implementation();

	shield->Throw(FQuat(Owner->GetControlRotation()).GetForwardVector());

	if(!!shield)
		shield->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);


}

void UCSkillAction_Sword::End_SkillAction_Implementation()
{
	Super::End_SkillAction_Implementation();

	State->SetIdleMode();
	State->OffSubActionMode();

	Movement->Move();
	Movement->DisableFixedCamera();

	SkillNum = 0;
}
