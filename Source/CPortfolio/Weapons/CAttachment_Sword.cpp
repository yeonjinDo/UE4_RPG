#include "Weapons/CAttachment_Sword.h"
#include "Global.h"
#include "Components/CSubStateComponent.h"


ACAttachment_Sword::ACAttachment_Sword()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Root);
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule", Root);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Weapons/Sword/SK_Sword.SK_Sword'");
	SkeletalMesh->SetSkeletalMesh(mesh);
	SkeletalMesh->SetCollisionProfileName("NoCollision");


}

void ACAttachment_Sword::BeginPlay()
{
	Super::BeginPlay();

	AttachTo("Holster_Sword");

	SkeletalMesh->SetVisibility(true);

	SubState = CHelpers::GetComponent<UCSubStateComponent>(OwnerCharacter);

}

void ACAttachment_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ACAttachment_Sword::OnBeginEquip_Implementation()
{
	Super::OnBeginEquip_Implementation();

	AttachTo("Hand_Sword");

}

void ACAttachment_Sword::OnUnequip_Implementation()
{
	Super::OnUnequip_Implementation();
	
	AttachTo("Holster_Sword");
}

void ACAttachment_Sword::Hidden_Implementation()
{
	Super::Hidden_Implementation();

	SkeletalMesh->SetVisibility(false);
}

void ACAttachment_Sword::Visible_Implementation()
{
	Super::Visible_Implementation();

	SkeletalMesh->SetVisibility(true);
}

void ACAttachment_Sword::Destroy_Implementation()
{
	Super::Destroy_Implementation();

	Destroy();
}
