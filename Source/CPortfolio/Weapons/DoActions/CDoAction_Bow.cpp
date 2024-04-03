#include "Weapons/DoActions/CDoAction_Bow.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Weapons/CAttachment.h"
#include "Weapons/AddOns/CArrow.h"
#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/CStateComponent.h"
#include "Weapons/CEquipment.h"
#include "Weapons/Attachments/CAttachment_Bow.h"

UCDoAction_Bow::UCDoAction_Bow()
{
	
}

void UCDoAction_Bow::BeginPlay(ACAttachment * InAttachment, UCEquipment * InEquipment, ACharacter * InOwner, const TArray<FDoActionData>& InDoActionData, const TArray<FHitData>& InHitData)
{
	Super::BeginPlay(InAttachment, InEquipment, InOwner, InDoActionData, InHitData);

	PoseableMesh = CHelpers::GetComponent<UPoseableMeshComponent>(InAttachment);
	SkeletalMesh = CHelpers::GetComponent<USkeletalMeshComponent>(InAttachment);
	
	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	
	if(!!bow)
		Bend = bow->GetBend();

	OriginLocation = PoseableMesh->GetBoneLocationByName("bow_string_mid", EBoneSpaces::ComponentSpace);

	bEquipped = InEquipment->GetEquipped();
}

void UCDoAction_Bow::DoAction()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(State->IsSubActionMode());

	Super::DoAction();

	DoActionDatas[0].DoAction(OwnerCharacter);

}

void UCDoAction_Bow::Begin_DoAction()
{
	Super::Begin_DoAction();

	bAttachedString = false;

	//휘어진 정도 0으로 돌려주고, 활 줄 원래 위치로 돌려주기
	*Bend = 0;
	PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);


	//화살 발사하기
	//붙어있는 화살 가져와서 
	ACArrow* arrow = GetAttachedArrow();
	if (!!arrow)
	{
		arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
		//KeepRelative로 하면 화살과 손의 간격으로 되어 0으로 바뀐다 World 그 자리에서 그대로 떨어뜨려야함 !

		arrow->OnHit.AddDynamic(this, &UCDoAction_Bow::OnArrowHit);
		arrow->OnEndPlay.AddDynamic(this, &UCDoAction_Bow::OnArrowEndPlay);

		ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
		if(!!player)
		{
			//Crosshair안에 화살을 날아가도록
			FVector crossHairLocation
					= UGameplayStatics::GetPlayerCameraManager(OwnerCharacter->GetWorld(), 0)->K2_GetActorLocation();
			FVector end = crossHairLocation
					+ UGameplayStatics::GetPlayerCameraManager(OwnerCharacter->GetWorld(), 0)->GetActorForwardVector()
					* ScaleForwardVector;

			TArray<AActor*> ignores;
			ignores.Add(OwnerCharacter);

			FHitResult hitResult;

			UKismetSystemLibrary::LineTraceSingle(OwnerCharacter->GetWorld(), crossHairLocation, end,
												ETraceTypeQuery::TraceTypeQuery2, false, ignores,
													EDrawDebugTrace::None, hitResult, true);

			if (hitResult.bBlockingHit == true)
			{
				end = hitResult.ImpactPoint;
			}

			spawnLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right_Arrow");

			FVector spawnDirection = end - spawnLocation;
			spawnRotation = UKismetMathLibrary::MakeRotFromX(spawnDirection);

			FVector forward = UKismetMathLibrary::GetForwardVector(spawnRotation);

			arrow->Shoot(forward);
		}
		else
		{
			FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
			arrow->Shoot(forward);
		}

	}
}

void UCDoAction_Bow::End_DoAction()
{
	Super::End_DoAction();

	CreateArrow();
}

void UCDoAction_Bow::OnBeginEquip()
{
	Super::OnBeginEquip();

	//활하고 충돌 막는거
	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CreateArrow();
}

void UCDoAction_Bow::OnUnequip()
{
	Super::OnUnequip();

	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	*Bend = 0;

	PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);

	//붙어있는 화살 제거 , 중간에 삭제된 화살이 있을 수 도 있어서 거꾸로 돌려야한다
	CheckFalse(Arrows.Num() > 0);
	
	for (int32 i = Arrows.Num() - 1; i >= 0; i--)
	{
		if(!!Arrows[i])
		{
			Arrows[i]->Destroy();
		}
	}

}

void UCDoAction_Bow::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	//활모양 매 프레임마다 캡쳐
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);

	bool bCheck = true;
	bCheck &= (*bEquipped == true);
	bCheck &= (bBeginAction == false);
	bCheck &= (bAttachedString == true);
	//*beEquipped는 true, bBeginAction은 false, bAttachedString은 true일 때 bCheck는 true
	CheckFalse(bCheck);

	FVector handLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right");
	PoseableMesh->SetBoneLocationByName("bow_string_mid", handLocation, EBoneSpaces::WorldSpace);

}

void UCDoAction_Bow::End_BowString()
{	//notify에서 콜
	*Bend = 100;
	bAttachedString = true ;
}

void UCDoAction_Bow::CreateArrow()
{
	if (World->bIsTearingDown == true)
		return;

	// SpawnActorDeferred : 값 설정 해주고 스폰 시켜주는것 
	FTransform transform;
	
	ACArrow* arrow = World->SpawnActorDeferred<ACArrow>(ArrowClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	CheckNull(arrow);

	//자기 자신 제외
	arrow->AddIgnoreActor(OwnerCharacter);

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	arrow->AttachToComponent(OwnerCharacter->GetMesh(), rule, "Hand_Bow_Right_Arrow");

	Arrows.Add(arrow);
	UGameplayStatics::FinishSpawningActor(arrow, transform);
}

ACArrow* UCDoAction_Bow::GetAttachedArrow()
{
	for (int32 i = Arrows.Num() - 1; i >= 0; i--)
	{
		if (!!Arrows[i]->GetAttachParentActor())
		{
			 return Arrows[i];
		}
	}

	return nullptr;
}

void UCDoAction_Bow::OnArrowHit(AActor* InCauser, ACharacter* InOtherCharacter)
{
	CheckFalse(HitDatas.Num() > 0);

	HitDatas[0].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);
}

void UCDoAction_Bow::OnArrowEndPlay(ACArrow* InDestroyer)
{
	Arrows.Remove(InDestroyer);
}