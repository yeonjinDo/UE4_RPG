#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"

void UCDoAction_Combo::DoAction()
{

	CheckTrue(DoActionDatas.Num() < 1);

	if(bEnable)
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse(State->IsIdleMode());

	Super::DoAction();
	DoActionDatas[Index].DoAction(OwnerCharacter);

}

void UCDoAction_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();

	if (OwnerCharacter->GetName() == "BP_CPlayer_C_0")
	{
		CheckFalse(bExist);
	}
	else       //enemy 랜덤콤보
	{	
		int32 random = UKismetMathLibrary::RandomInteger(2);

		if (random != 1)
			return;
	}

	bExist = false;

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(OwnerCharacter);
	if (!!enemy)
	{
		UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(enemy);
		if (aiState->IsActionMode() == false)
			return;

	}
	DoActionDatas[++Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::End_DoAction()
{
	Super::End_DoAction();

	Index = 0;
}

void UCDoAction_Combo::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	if(HittedRotation)
	{
		AController* controller = OwnerCharacter->GetController<AController>();
		controller->SetControlRotation(FMath::RInterpTo(controller->GetControlRotation(), TargetRotation, InDeltaTime, 7.0f));

		if (controller->GetControlRotation().Equals(TargetRotation, 0.5))
			HittedRotation = false;
	}

		
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);
	CheckNull(InOther);

	//CLog::Log(InOther->GetName());
	//InOther->TakeDamage(20, FDamageEvent(), InAttacker->GetController(), InAttackCauser);
	
	for(ACharacter* hitted : Hitted)
		CheckTrue(hitted == InOther);

	Hitted.AddUnique(InOther);

	CheckTrue(HitDatas.Num() - 1 < Index);
	HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);
}

void UCDoAction_Combo::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();

	//공격했을때 공격 방향으로 카메라 잡아주는 거
	float angle = -2.0f;
	ACharacter* candidate = nullptr;

	for (ACharacter* hitted: Hitted)
	{
		FVector direction = hitted->GetActorLocation() - OwnerCharacter->GetActorLocation();
		direction = direction.GetSafeNormal2D();

		//카메라와 전방방향 가장 가까운걸 찾는다
		FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();

		//dot : 내적
		float dot = FVector::DotProduct(direction, forward);
		if(dot >= angle)
		{
			angle = dot;
			candidate = hitted;
		}//가장 1에 가까운값이 나온다 = 정면, 내적이니까 !

	}

	if(!!candidate)
	{
		HittedRotation = true;
		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), candidate->GetActorLocation());
		//한번에 팍 돌리는데, 여기에 틱 하나 두고 rinterpto로 하면 훨씬 부드러워진다 -> 완료!

		TargetRotation = FRotator(0, rotator.Yaw, 0);

		//AController* controller = OwnerCharacter->GetController<AController>();
		//controller->SetControlRotation(target);
	}
		Hitted.Empty();

}
