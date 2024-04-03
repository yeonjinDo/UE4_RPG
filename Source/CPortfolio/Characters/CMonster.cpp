#include "Characters/CMonster.h"
#include "Global.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CStatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CWeaponStructures.h"
#include "Widgets/CUserWidget_Label.h"


ACMonster::ACMonster()
{

	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Bloodthirsty_beetle/Mesh/SK_Bloodthirsty_beetle.SK_Bloodthirsty_beetle'");
	GetMesh()->SetSkeletalMesh(mesh);

}

void ACMonster::BeginPlay()
{
	Super::BeginPlay();

	State->OnStateTypeChanged.AddDynamic(this, &ACMonster::OnStateTypeChanged);


#if WITH_EDITOR
	const FString name = TEXT("작은 용");
	label->UpdateName(FText::FromString(name));
#endif
}

void ACMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void ACMonster::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hitted:	Hitted();   break;
	case EStateType::Dead:		Dead();     break;
	}
}

float ACMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage.Power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;

	CheckTrueResult(Status->IsDead(), 0);
	State->SetHittedMode();

	return damage;
}

void ACMonster::Hitted()
{
	if (Status->IsDead())
	{
		//사망 처리
		State->SetDeadMode();

		return;
	}

	Status->Damage(Damage.Power);	//HP 대미지 처리
	Damage.Power = 0;


	if (!!Damage.Event && !!Damage.Event->HitData)
	{
		FHitData* data = Damage.Event->HitData;

		if(Behavior->IsActionMode() == false)
			Montages->PlayHittedMode();
	
		data->PlayHitStop(GetWorld());
		data->PlaySoundWave(this);
		data->PlayEffect(GetWorld(), Damage.Causer->GetActorLocation(), GetActorRotation());


		if (Status->IsDead() == false)	// 죽으면서 아래로 빠지는 것 방지함
		{
			//launch character
			FVector  start = GetActorLocation();
			FVector target = Damage.Character->GetActorLocation();
			FVector direction = target - start;
			direction.Normalize();

		
			LookAtRotation = UKismetMathLibrary::FindLookAtRotation(start, target);
			//SetActorRotation(FRotator(0, LookAtRotation.Yaw, 0));	//적이 잘못맞아서 앞뒤로 고꾸라지는것 방지

			TargetRotation = FRotator(0, LookAtRotation.Yaw, 0);

		}

	}


	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;


}

void ACMonster::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Montages->PlayDeadMode();

}

void ACMonster::End_Hitted()
{
	IICharacter::End_Hitted();

	State->SetIdleMode();
	Behavior->SetWaitMode();
}


void ACMonster::End_Dead()
{
	Destroy();
}

FGenericTeamId ACMonster::GetGenericTeamId() const
{
	return FGenericTeamId(5);
}


