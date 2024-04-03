#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Global.h"
#include "Characters/CAnimInstance_Bow.h"
#include "Components/CSubStateComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"


ACAttachment_Bow::ACAttachment_Bow()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Root);
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &PoseableMesh, "PoseableMesh", Root);


	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Weapons/ElvenBow/SK_ElvenBow.SK_ElvenBow'");
	SkeletalMesh->SetSkeletalMesh(mesh);
	SkeletalMesh->SetCollisionProfileName("NoCollision");

	TSubclassOf<UCAnimInstance_Bow> animInstance;
	CHelpers::GetClass<UCAnimInstance_Bow>(&animInstance, "AnimBlueprint'/Game/Weapons/Bow/ABP_Bow.ABP_Bow_C'");
	SkeletalMesh->SetAnimInstanceClass(animInstance);
	
}

void ACAttachment_Bow::BeginPlay()
{
	Super::BeginPlay();

	AttachTo("Holster_Bow");

	SkeletalMesh->SetVisibility(false);
	
	PoseableMesh->SetSkeletalMesh(SkeletalMesh->SkeletalMesh);
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);

	SubState = CHelpers::GetComponent<UCSubStateComponent>(OwnerCharacter);
}

void ACAttachment_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAttachment_Bow::OnBeginEquip_Implementation()
{
	Super::OnBeginEquip_Implementation();

	AttachTo("Hand_Bow_Left");


	//활에서 카메라가 바닥 아래로 넘어가지 않도록 각도 조절
	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	if (!!controller)
	{
		//원래 시야각 저장
		OriginViewPitchRange.X = controller->PlayerCameraManager->ViewPitchMin;
		OriginViewPitchRange.Y = controller->PlayerCameraManager->ViewPitchMax;
		//
		controller->PlayerCameraManager->ViewPitchMin = ViewPitchRange.X;
		controller->PlayerCameraManager->ViewPitchMax = ViewPitchRange.Y;
	}
}


void ACAttachment_Bow::OnUnequip_Implementation()
{
	Super::OnUnequip_Implementation();

	AttachTo("Holster_Bow");

	//시야각 되돌려주기
	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	if (!!controller)
	{
		controller->PlayerCameraManager->ViewPitchMin = OriginViewPitchRange.X;
		controller->PlayerCameraManager->ViewPitchMax = OriginViewPitchRange.Y;
	}
}

void ACAttachment_Bow::OnHidden_Implementation()
{
	Super::OnHidden_Implementation();

	PoseableMesh->SetVisibility(false);
}

void ACAttachment_Bow::OnVisible_Implementation()
{
	Super::OnVisible_Implementation();

	PoseableMesh->SetVisibility(true);
}

void ACAttachment_Bow::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();

	Destroy();
}

float* ACAttachment_Bow::GetBend()
{
	return Cast<UCAnimInstance_Bow>(SkeletalMesh->GetAnimInstance())->GetBend();
}
