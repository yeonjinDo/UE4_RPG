#include "Weapons/SubActions/CSubAction_Fist.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/AddOns/CGhostTrail.h"


UCSubAction_Fist::UCSubAction_Fist()
{
	CoolTime = 10.0f;

	OnSubActionCoolDown.AddDynamic(this, &UCSubAction_Fist::OnCoolDown);

}

void UCSubAction_Fist::Pressed()
{
	
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

	GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, Owner);

	ActionData.DoAction(Owner);

	SkillNum = 1;
}

void UCSubAction_Fist::Begin_SubAction_Implementation()
{
	Super::Begin_SubAction_Implementation();

	CHelpers::DelayWorldTime(Owner, 0.2f, false);



	//이벤트 제거
	Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");

	//이벤트 연결
	Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCSubAction_Fist::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSubAction_Fist::OnAttachmentBeginOverlap);
}

void UCSubAction_Fist::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();

	CHelpers::DelayWorldTime(Owner, 1, true);

	//이벤트 제거
	Attachment->OnAttachmentEndCollision.Remove(this, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");

	//이벤트 연결
	Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	if (OnSubActionCoolDown.IsBound())
		OnSubActionCoolDown.Broadcast();


	State->SetIdleMode();
	State->OffSubActionMode();

	Movement->Move();
	Movement->DisableFixedCamera();


	GhostTrail->Destroy();

	HitIndex = 0;
}

void UCSubAction_Fist::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckNull(InOther);

	for (ACharacter* character : Hitted)
		CheckTrue(character == InOther);

	Hitted.AddUnique(InOther);

	CheckTrue(HitIndex >= HitDatas.Num());
	HitDatas[HitIndex].SendDamage(Owner, InAttackCauser, InOther);

}

void UCSubAction_Fist::OnAttachmentEndCollision()
{
	Hitted.Empty();

	HitIndex++;

}

void UCSubAction_Fist::OnCoolDown()
{
	FTimerDelegate delegate = FTimerDelegate::CreateLambda([&]()
	{
		if (CurrentTime <= CoolTime)
			CurrentTime += 0.1f;

	});

	Owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle, delegate, 0.1, true, 0);

}
 