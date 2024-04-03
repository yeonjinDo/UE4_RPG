#include "Weapons/CWeaponStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CMovementComponent.h"
#include "Animation/AnimMontage.h"


void FDoActionData::DoAction(ACharacter* InOwner)
{
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);

	if(!!movement)
	{
		if (bFixedCamera)
			movement->EnableFixedCamera();

		if (bCanMove == false)
			movement->Stop();
	}

	if (!!Montage)	//몽타주가 설정되어있다면 플레이
		InOwner->PlayAnimMontage(Montage, PlayRate);
}


////////////////////////////////////////////////////////////////////////////////////////

void FHitData::SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	FActionDamageEvent e;
	e.HitData = this;

	InOther->TakeDamage(Power, e, InAttacker->GetController(), InAttackCauser);
}


void FHitData::PlayMontage(ACharacter * InOwner)
{

	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);

}

void FHitData::PlayHitStop(UWorld* InWorld)
{
	CheckTrue(FMath::IsNearlyZero(StopTime));

	TArray<APawn*> pawns;
	for(AActor* actor: InWorld->GetCurrentLevel()->Actors)
	{
		APawn* pawn = Cast<ACharacter>(actor);

		if(!!pawn)
		{
			pawn->CustomTimeDilation = 1e-3f;	// 1/3배

			pawns.Add(pawn);
		}
	}

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
	{
		for (APawn* pawn : pawns)
			pawn->CustomTimeDilation = 1;

	});

	FTimerHandle handle;
	InWorld->GetTimerManager().SetTimer(handle, timerDelegate, StopTime, false);

}

void FHitData::PlaySoundWave(ACharacter* InOwner)
{
	CheckNull(Sound);

	UWorld*    world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);

}

void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);		//크기보정
	transform.AddToTranslation(InLocation); //location만큼 이동

	CHelpers::PlayEffect(InWorld, Effect, transform);

}


void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation)
{
	CheckNull(Effect);

	FTransform transform;

	//회전방향만큼 이동한걸로 계산, InLocation에는 Actor 위치가 들어옴
	transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));
	transform.SetScale3D(EffectScale);		

	CHelpers::PlayEffect(InWorld, Effect, transform);


}
