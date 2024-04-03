#include "Weapons/Attachments/CAttachment_Shield.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CSubStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapons/CEquipment.h"


ACAttachment_Shield::ACAttachment_Shield()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Root);
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", SkeletalMesh);
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");


	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Weapon_Pack/Skeletal_Mesh/SK_Shield.SK_Shield'");
	SkeletalMesh->SetSkeletalMesh(mesh);
	

}

void ACAttachment_Shield::BeginPlay()
{
	Super::BeginPlay();

	Projectile->Deactivate();

	AttachTo("Back_Shield");

	SkeletalMesh->SetVisibility(true);
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	SubState = CHelpers::GetComponent<UCSubStateComponent>(OwnerCharacter);
	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment_Shield::OnOverlap);
}

void ACAttachment_Shield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (SubState->IsDashMode())
	//	SkeletalMesh->SetVisibility(false);
	//else
	//	SkeletalMesh->SetVisibility(true);


	if(bThrow == false)
	{

		//Defend에서 방패를 붙이는 작업 // 방패를 팔에 붙이는 경우
		if ((State->IsDefendMode() || State->IsParryingMode()) && bDefendMode == false)
		{
			bDefendMode = true;
			this->OnBeginEquip_Implementation();
			AttachTo("Hand_Shield");
		}

		//방패를 등에 붙이는 경우
		else if (State->IsDefendMode() == false && State->IsParryingMode() == false
			&& Weapon->IsSwordMode() == false && bDefendMode == true)
		{	//조건이 좀 지저분한거같다 ,, ;;; 
			AttachTo("Back_Shield");
			this->OnUnequip_Implementation();
			bDefendMode = false;
		}

	}


	if(bThrow)
	{

		//LineTrace -> 각도 알아내려고
		TArray<AActor*> ignores;
		ignores.Add(OwnerCharacter);

		FVector start = this->GetActorLocation();
		FVector end = start + OwnerCharacter->GetActorForwardVector() * 500;

		UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery2, false, ignores,
			DebugType, HitResults, true, FLinearColor::Red, FLinearColor::Green);

		angle = HitResults.Normal;

		if (angle == FVector(0,0,0))
			angle = (-OwnerCharacter->GetActorForwardVector());

	}

	
}

void ACAttachment_Shield::OnBeginEquip_Implementation()
{
	Super::OnBeginEquip_Implementation();

	AttachTo("Hand_Shield");
}

void ACAttachment_Shield::OnUnequip_Implementation()
{
	Super::OnUnequip_Implementation();

	AttachTo("Back_Shield");
}

void ACAttachment_Shield::OnHidden_Implementation()
{
	Super::OnHidden_Implementation();

	SkeletalMesh->SetVisibility(false);
}

void ACAttachment_Shield::OnVisible_Implementation()
{
	Super::OnVisible_Implementation();

	SkeletalMesh->SetVisibility(true);
}

void ACAttachment_Shield::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();

	Destroy();
}

void ACAttachment_Shield::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OwnerCharacter->GetName() == OtherActor->GetName())
	{		
		return;
	}

	//projectile
	Projectile->Velocity = angle * Speed;

	//hit
	if(!!OtherActor)
	{
		HittedCharacter = Cast<ACharacter>(OtherActor);
		CheckNull(HittedCharacter);

		HitData.SendDamage(OwnerCharacter, this, HittedCharacter);
	}

}

void ACAttachment_Shield::Throw(const FVector& InForward)
{
	bThrow = true;

	Projectile->ProjectileGravityScale = 0;
	Projectile->Velocity = InForward * Projectile->InitialSpeed;
	Projectile->Activate();

	Box->SetCollisionProfileName("OverlapAll");
	Box->SetEnableGravity(false);
	SetActorRotation(FRotator(0, 0, -90));

}

void ACAttachment_Shield::Catch()
{	//노티파이에서 호출

	bThrow = false;
	Box->SetCollisionProfileName("NoCollision");
	AttachTo("Hand_Shield");
	
	Projectile->Velocity = FVector::ZeroVector;
}
