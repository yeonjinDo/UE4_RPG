#include "Weapons/QSkillActions/CSkillAction_Sword.h"
#include "Global.h"
#include "Weapons/Attachments/CAttachment_Shield.h"
#include "Weapons/AddOns/CShield.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapons/CDoAction.h"
#include "Kismet/KismetMathLibrary.h"

void UCSkillAction_Sword::OnCoolDown()
{
	FTimerDelegate delegate = FTimerDelegate::CreateLambda([&]()
	{
		if (CurrentTime <= CoolTime)
			CurrentTime += 0.1f;
	});

	Owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle, delegate, 0.1, true, 0);

}

UCSkillAction_Sword::UCSkillAction_Sword()
{
	CoolTime = 12.0f;

	OnSkillActionCoolDown.AddDynamic(this, &UCSkillAction_Sword::OnCoolDown);

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
	//CoolTime
	if (CurrentTime <= CoolTime)
	{
		bInAction = false;

		return;
	}

	CurrentTime = 0;
	//CoolTime


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


	if (OnSkillActionCoolDown.IsBound())
		OnSkillActionCoolDown.Broadcast();

	State->SetIdleMode();
	State->OffSubActionMode();

	Movement->Move();
	Movement->DisableFixedCamera();

	SkillNum = 0;
}
