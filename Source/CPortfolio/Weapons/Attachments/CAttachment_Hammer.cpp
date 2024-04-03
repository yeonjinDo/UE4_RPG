#include "CAttachment_Hammer.h"
#include "Global.h"

ACAttachment_Hammer::ACAttachment_Hammer()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Skeletal, "SkeletalMesh", Root);
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Cap, "Capsule", Root);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Weapons/GreateHammer/SM_GreatHammer_SM_GreatHammer.SM_GreatHammer_SM_GreatHammer'");
	Skeletal->SetSkeletalMesh(mesh);
	Skeletal->SetCollisionProfileName("NoCollision");

}

void ACAttachment_Hammer::BeginPlay()
{
	Super::BeginPlay();

	AttachTo("Hand_Hammer");

	Skeletal->SetVisibility(false);

}

void ACAttachment_Hammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(Skeletal->IsVisible());

	if(bEquip == false)
	{
		Skeletal->SetVisibility(false);
	}
}

void ACAttachment_Hammer::OnBeginEquip_Implementation()
{
	Super::OnBeginEquip_Implementation();

	bEquip = true;
	AttachTo("Hand_Hammer");
	Skeletal->SetVisibility(true);
}

void ACAttachment_Hammer::OnUnequip_Implementation()
{
	Super::OnUnequip_Implementation();

	bEquip = false;
	Skeletal->SetVisibility(false);
}

void ACAttachment_Hammer::OnHidden_Implementation()
{
	Super::OnHidden_Implementation();

	Skeletal->SetVisibility(false);
}

void ACAttachment_Hammer::OnVisible_Implementation()
{
	Super::OnVisible_Implementation();

	Skeletal->SetVisibility(true);
}

void ACAttachment_Hammer::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();

	Destroy();
}
