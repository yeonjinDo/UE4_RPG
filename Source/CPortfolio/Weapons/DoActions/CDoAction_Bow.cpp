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

	//�־��� ���� 0���� �����ְ�, Ȱ �� ���� ��ġ�� �����ֱ�
	*Bend = 0;
	PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);


	//ȭ�� �߻��ϱ�
	//�پ��ִ� ȭ�� �����ͼ� 
	ACArrow* arrow = GetAttachedArrow();
	if (!!arrow)
	{
		arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
		//KeepRelative�� �ϸ� ȭ��� ���� �������� �Ǿ� 0���� �ٲ�� World �� �ڸ����� �״�� ����߷����� !

		arrow->OnHit.AddDynamic(this, &UCDoAction_Bow::OnArrowHit);
		arrow->OnEndPlay.AddDynamic(this, &UCDoAction_Bow::OnArrowEndPlay);

		ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
		if(!!player)
		{
			//Crosshair�ȿ� ȭ���� ���ư�����
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

	//Ȱ�ϰ� �浹 ���°�
	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CreateArrow();
}

void UCDoAction_Bow::OnUnequip()
{
	Super::OnUnequip();

	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	*Bend = 0;

	PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);

	//�پ��ִ� ȭ�� ���� , �߰��� ������ ȭ���� ���� �� �� �־ �Ųٷ� �������Ѵ�
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

	//Ȱ��� �� �����Ӹ��� ĸ��
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);

	bool bCheck = true;
	bCheck &= (*bEquipped == true);
	bCheck &= (bBeginAction == false);
	bCheck &= (bAttachedString == true);
	//*beEquipped�� true, bBeginAction�� false, bAttachedString�� true�� �� bCheck�� true
	CheckFalse(bCheck);

	FVector handLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right");
	PoseableMesh->SetBoneLocationByName("bow_string_mid", handLocation, EBoneSpaces::WorldSpace);

}

void UCDoAction_Bow::End_BowString()
{	//notify���� ��
	*Bend = 100;
	bAttachedString = true ;
}

void UCDoAction_Bow::CreateArrow()
{
	if (World->bIsTearingDown == true)
		return;

	// SpawnActorDeferred : �� ���� ���ְ� ���� �����ִ°� 
	FTransform transform;
	
	ACArrow* arrow = World->SpawnActorDeferred<ACArrow>(ArrowClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	CheckNull(arrow);

	//�ڱ� �ڽ� ����
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