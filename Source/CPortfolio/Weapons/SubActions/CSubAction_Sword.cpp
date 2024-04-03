#include "Weapons/SubActions/CSubAction_Sword.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/AddOns/CGhostTrail.h"

UCSubAction_Sword::UCSubAction_Sword()
{
	CoolTime = 3.0f;

	OnSubActionCoolDown.AddDynamic(this, &UCSubAction_Sword::OnCoolDown);

}

void UCSubAction_Sword::Pressed()
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

	GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, Owner);

	ActionData.DoAction(Owner);

	SkillNum = 2;
}


void UCSubAction_Sword::Begin_SubAction_Implementation()
{
	Super::Begin_SubAction_Implementation();

	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSubAction_Sword::OnAttachmentBeginOverlap);

	
	bMoving = true;

	Start = Owner->GetActorLocation();
	End = Start + Owner->GetActorForwardVector() * Distance;


	float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FRotator rotation = Owner->GetActorRotation();


	TArray<AActor*> ignores;
	ignores.Add(Owner);

	TArray<FHitResult> hitResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery3);

	UKismetSystemLibrary::BoxTraceMultiForObjects(Owner->GetWorld(), Start, End, FVector(0, radius, height), rotation,
													objects, false, ignores, DrawDebug, hitResults, true);
	for(const FHitResult& hitResult : hitResults)
	{
		ACharacter* character = Cast<ACharacter>(hitResult.GetActor());

		if(!!character)
		{
			character->GetCapsuleComponent()->SetCollisionProfileName("Sword_SubAction");

			Overlapped.Add(character);
		}
	}

	FHitResult lineHitResult;
	UKismetSystemLibrary::LineTraceSingle(Owner->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, false, ignores, DrawDebug, lineHitResult, true);

	//벽에 너무 붙는거 방지
	if(lineHitResult.bBlockingHit)
	{
		FVector direction = (End - Start).GetSafeNormal2D();
		End = lineHitResult.Location - (direction *radius * 2);

		if (DrawDebug == EDrawDebugTrace::ForDuration)
			DrawDebugSphere(Owner->GetWorld(), End, radius * 2, 20, FColor::Magenta, true, 2);

	}

	
	//디버깅/ 시작위치에서 끝위치까지 화살표 그리는거
	if (DrawDebug == EDrawDebugTrace::ForDuration)
		DrawDebugDirectionalArrow(Owner->GetWorld(), Start, End, 25, FColor::Green, true, 5, 0, 3);
	//


}

void UCSubAction_Sword::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();


	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	if (OnSubActionCoolDown.IsBound())
		OnSubActionCoolDown.Broadcast();

	bMoving = false;

	State->SetIdleMode();
	State->OffSubActionMode();

	Movement->Move();
	Movement->DisableFixedCamera();

	//collision profile 바꿨던거 되돌려주기
	for (ACharacter* character : Overlapped)
		character->GetCapsuleComponent()->SetCollisionProfileName("Pawn");
	//

	if (!!GhostTrail)
		GhostTrail->Destroy();

	Overlapped.Empty();
	Hitted.Empty();

}

void UCSubAction_Sword::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	//이동모드일때만 수행하도록
	CheckFalse(bMoving);

	//End에 정확하게 도달하도록
	FVector location = Owner->GetActorLocation();
	float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();

	if(location.Equals(End, radius))
	{
		bMoving = false;
		Start = End = Owner->GetActorLocation();

		return;
	}

	//2D평면상에서 움직일거기 때문에 GetSafeNormal2D()
	FVector direction = (End - Start).GetSafeNormal2D();
	Owner->AddActorWorldOffset(direction * Speed * InDeltaTime, true);


}

float UCSubAction_Sword::GetPercent()
{
	return CurrentTime / CoolTime;
}

void UCSubAction_Sword::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckNull(InOther);

	for (ACharacter* character : Hitted)
		CheckTrue(character == InOther);

	Hitted.AddUnique(InOther);
	HitData.SendDamage(Owner, InAttackCauser, InOther);

}

void UCSubAction_Sword::OnCoolDown()
{
	FTimerDelegate delegate = FTimerDelegate::CreateLambda([&]()
	{
		if (CurrentTime <= CoolTime)
			CurrentTime += 0.1f;


	});

	Owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle, delegate, 0.1, true, 0);
	
}
