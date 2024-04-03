#include "Weapons/SubActions/CSubAction_Hammer.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/AddOns/CGhostTrail.h"
#include "Weapons/AddOns/CAura.h"


void UCSubAction_Hammer::OnCoolDown()
{
	FTimerDelegate delegate = FTimerDelegate::CreateLambda([&]()
	{
		if (CurrentTime <= CoolTime)
			CurrentTime += 0.1f;

	});

	Owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle, delegate, 0.1, true, 0);

}

UCSubAction_Hammer::UCSubAction_Hammer()
{
	CoolTime = 5.0f;

	OnSubActionCoolDown.AddDynamic(this, &UCSubAction_Hammer::OnCoolDown);

}

void UCSubAction_Hammer::Pressed()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSubActionMode());

	Super::Pressed();

	

	if (CurrentTime <= CoolTime)
	{
		bInAction = false;

		return;
	}

	CurrentTime = 0;

	

	CHelpers::DelayWorldTime(Owner, 0.2f, false);

	State->SetActionMode();
	State->OnSubActionMode();

	GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, Owner);

	ActionData.DoAction(Owner);

	SkillNum = 3;
}

void UCSubAction_Hammer::Begin_SubAction_Implementation()
{
	Super::Begin_SubAction_Implementation();

	CHelpers::DelayWorldTime(Owner, 1.0f, true);

	//Aura 나이아가라 스폰
	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;
	transform.SetLocation(Owner->GetActorLocation());
	transform.AddToTranslation(AuraLocation);
	transform.SetRotation(FQuat(Owner->GetActorRotation()));

	Owner->GetWorld()->SpawnActor<ACAura>(AuraClass, transform, params);
}

void UCSubAction_Hammer::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();
	if (OnSubActionCoolDown.IsBound())
		OnSubActionCoolDown.Broadcast();

	State->SetIdleMode();
	State->OffSubActionMode();

	Movement->Move();
	Movement->DisableFixedCamera();


	if (!!GhostTrail)
		GhostTrail->Destroy();
}
