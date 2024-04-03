#include "Weapons/SubActions/CSubAction_Around.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/AddOns/CNotify_Around.h"
#include "Weapons/AddOns/CThornObject.h"


void UCSubAction_Around::OnCoolDown()
{
	FTimerDelegate delegate = FTimerDelegate::CreateLambda([&]()
	{
		if (CurrentTime <= CoolTime)
			CurrentTime += 0.1f;

	});

	Owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle, delegate, 0.1, true, 0);

}

UCSubAction_Around::UCSubAction_Around()
{
	CoolTime = 5.0f;

	OnSubActionCoolDown.AddDynamic(this, &UCSubAction_Around::OnCoolDown);
}

void UCSubAction_Around::Pressed()
{
	Super::Pressed();

	
	//CoolTime
	if (CurrentTime <= CoolTime)
	{
		bInAction = false;

		return;
	}

	//CoolTime


	CheckTrue(State->IsSubActionMode());

	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;

	FVector ownerLocation = Owner->GetActorLocation();
	transform.SetLocation(ownerLocation);

	if (!!notifyClass)
		notifyClass->Destroy();

	notifyClass = Owner->GetWorld()->SpawnActor<ACNotify_Around>(NotifyClass, transform, params);


	SkillNum = 5;
}

void UCSubAction_Around::Released()
{
	Super::Released();

	//CoolTime
	if (CurrentTime <= CoolTime)
	{
		bInAction = false;

		return;
	}

	CheckTrue(State->IsSubActionMode());
	State->OnSubActionMode();

	if(!!notifyClass)
	{
		notifyLocation = notifyClass->GetNotifyLocation();
		notifyRotation = notifyClass->GetNotifyRotation();
		notifyClass->Destroy();
	}

	if (OnSubActionCoolDown.IsBound())
		OnSubActionCoolDown.Broadcast();


	//기본 동작 플레이
	ActionData.DoAction(Owner);
}

void UCSubAction_Around::Begin_SubAction_Implementation()
{
	Super::Begin_SubAction_Implementation();

	//thorn 스폰
	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;

	FVector ownerLocation = notifyLocation;
	ownerLocation.Z += 10;
	
	transform.SetLocation(ownerLocation);

	Owner->GetWorld()->SpawnActor<ACThorn_Floor>(ObjectClass, transform, params);
}

void UCSubAction_Around::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();

	if (OnSubActionCoolDown.IsBound())
		OnSubActionCoolDown.Broadcast();

	CurrentTime = 0;

	State->OffSubActionMode();
	State->SetIdleMode();
	Movement->Move();

}
