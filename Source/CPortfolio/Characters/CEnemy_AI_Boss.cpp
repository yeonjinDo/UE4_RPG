#include "Characters/CEnemy_AI_Boss.h"
#include "Global.h"
#include "Components/CMovementComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CWeaponStructures.h"
#include "Widgets/CUserWidget_BossStatus.h"
#include "Widgets/CUserWidget_Label.h"
#include "Widgets/CUserWidget_PlayerStatus.h"
#include "Widgets/CUserWidget_State.h"

ACEnemy_AI_Boss::ACEnemy_AI_Boss()
{
	
#if WITH_EDITOR
	
	CHelpers::GetClass<UCUserWidget_BossStatus>(&HpBar, "WidgetBlueprint'/Game/Widgets/WB_BossStatus.WB_BossStatus_C'");

#endif


}


void ACEnemy_AI_Boss::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR

	widget = CreateWidget(GetWorld(), HpBar);
	
	if (!!widget)
	{
		widget->AddToViewport();
	}


	FString name = TEXT("분노한 장군");
	label->UpdateName(FText::FromString(name));

#endif

}

void ACEnemy_AI_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITOR

	UCUserWidget_BossStatus* hpBar = Cast<UCUserWidget_BossStatus>(widget);
	if (!!hpBar)
	{
		hpBar->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	}

#endif

}



void ACEnemy_AI_Boss::Hitted()
{
	//State->IsHittedMode();

	Status->Damage(Damage.Power);
	Damage.Power = 0;

	
	Change_Color(this, FLinearColor::Red);

	FTimerDelegate timerDelegate;
	timerDelegate.BindUFunction(this, "RestoreColor");

	GetWorld()->GetTimerManager().SetTimer(RestoreColor_TimerHandle, timerDelegate, 0.2f, false);


	if (!!Damage.Event && !!Damage.Event->HitData)
	{
		FHitData* data = Damage.Event->HitData;

		data->PlayHitStop(GetWorld());
		data->PlayEffect(GetWorld(), Damage.Causer->GetActorLocation(), GetActorRotation());



		if (Status->IsDead() == false)	// 죽으면서 아래로 빠지는 ㅠ것 방지함
		{
			//launch character
			FVector  start = GetActorLocation();
			FVector target = Damage.Character->GetActorLocation();
			FVector direction = target - start;
			direction.Normalize();

		}

	}


	if (Status->IsDead())
	{
		//사망 처리
		State->SetDeadMode();

		return;
	}


	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;

	State->SetIdleMode();
}

void ACEnemy_AI_Boss::End_Action()
{
	Behavior->SetIdle();
	EndAction = true;

	ACharacter* target = Behavior->GetTarget();
	if (!!target)
	{
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target->GetActorLocation());
		SetActorRotation(Rotation);
	}
}

void ACEnemy_AI_Boss::End_Dash()
{
	Behavior->SetIdle();
	Movement->OnEndDash();

	if (Weapon->OnWeaponVisible.IsBound())
		Weapon->OnWeaponVisible.Broadcast();

}

void ACEnemy_AI_Boss::End_Dead()
{
	Super::End_Dead();
}

