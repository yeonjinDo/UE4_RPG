#include "Characters/CEnemy.h"
#include "Global.h"
#include "CPlayer.h"
#include "Blueprint/UserWidget.h"
#include "Components/CAirComboComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CDefendComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CStatusComponent.h"
#include "Weapons/CWeaponStructures.h"
#include "Widgets/CUserWidget_Damage.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>   (this, &State,    "State");
	CHelpers::CreateActorComponent<UCSubStateComponent>(this, &SubState, "SubState");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCStatusComponent>  (this, &Status,   "Status");
	CHelpers::CreateActorComponent<UCDefendComponent>  (this, &Defend,   "Defend");
	CHelpers::CreateActorComponent<UCAirComboComponent>(this, &AirCombo, "AirCombo");




#if WITH_EDITOR
	CHelpers::GetClass<UCUserWidget_Damage>(&DamageUI, "WidgetBlueprint'/Game/Widgets/WB_Damage.WB_Damage_C'");
	
#endif
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	//IICharacter 상속
	Create_DynamicMaterial(this);
	Change_Color(this, OriginColor);


#if WITH_EDITOR
	
#endif

}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State->IsHittedMode())
	{
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 3.0f));
	}
}

float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage.Power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;

	State->SetHittedMode();

	return damage;
}

void ACEnemy::Hitted()
{
	CheckTrue(State->IsDeadMode());
	Status->Damage(Damage.Power);	//HP 대미지 처리

#if WITH_EDITOR
	ACEnemy* attacker = Cast<ACEnemy>(Damage.Causer->GetOwner());
	if(attacker == nullptr)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		UUserWidget* DamageWidget = CreateWidget(controller, DamageUI);
		UCUserWidget_Damage* damageWidget = Cast<UCUserWidget_Damage>(DamageWidget);

		if (damageWidget)
			{
				damageWidget->SpawnDamage(Damage.Power, GetCapsuleComponent()->GetComponentLocation());

				if (damageWidget->IsInViewport() == false)
				{
					damageWidget->AddToViewport();
				}

			}
		
	}
#endif
	Damage.Power = 0;

	//Change_Color
	Change_Color(this, FLinearColor::Red);

	FTimerDelegate timerDelegate;
	timerDelegate.BindUFunction(this, "RestoreColor");

	GetWorld()->GetTimerManager().SetTimer(RestoreColor_TimerHandle, timerDelegate, 0.2f, false);
	//


	//AirCombo 
	ACPlayer* player = Cast<ACPlayer>(Damage.Character);
	if (!!player)
	{
		Player = player;
		UCSubStateComponent* playerState = CHelpers::GetComponent<UCSubStateComponent>(player);

		if (playerState->IsAirComboMode())
		{
			SubState->SetAirComboMode();
			player->Launch(true);
		}
	}
	//


	if(!!Damage.Event && !!Damage.Event->HitData)
	{
		FHitData* data = Damage.Event->HitData;

		data->PlayMontage(this);
		data->PlayHitStop(GetWorld());
		data->PlaySoundWave(this);
		data->PlayEffect(GetWorld(), /*Damage.Causer->*/GetActorLocation(), GetActorRotation());

		
		if(Status->IsDead() == false)	// 죽으면서 아래로 빠지는 것 방지함
		{
			//launch character
			FVector  start = GetActorLocation();
			FVector target = Damage.Character->GetActorLocation();
			FVector direction = target - start;
			direction.Normalize();

			if(SubState->IsAirComboMode() == false)
				LaunchCharacter(-direction * data->Launch, false, false);

			LookAtRotation = UKismetMathLibrary::FindLookAtRotation(start, target);
			//SetActorRotation(FRotator(0, LookAtRotation.Yaw, 0));	//적이 잘못맞아서 앞뒤로 고꾸라지는것 방지
			TargetRotation = FRotator(0, LookAtRotation.Yaw, 0);

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

}

void ACEnemy::End_Hitted()
{
	State->SetIdleMode();

	GetCharacterMovement()->GravityScale = 1;


#if WITH_EDITOR

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UUserWidget* DamageWidget = CreateWidget(controller, DamageUI);

	UCUserWidget_Damage* damageWidget = Cast<UCUserWidget_Damage>(DamageWidget);
	if (!!damageWidget)
	{
		damageWidget->RemoveFromViewport(); 
	}
#endif

}

void ACEnemy::RestoreColor()
{
	Change_Color(this, OriginColor);

	//타이머 사용 완료시 제거 
	GetWorld()->GetTimerManager().ClearTimer(RestoreColor_TimerHandle);
}

void ACEnemy::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Montages->PlayDeadMode();

}

void ACEnemy::End_Dead()
{
	Destroy();
}
