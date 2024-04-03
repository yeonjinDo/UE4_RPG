#include "Weapons/AddOns/CArrow.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/DoActions/CDoAction_Bow.h"


ACArrow::ACArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");
	
	Projectile->ProjectileGravityScale = 0;	//�߷� ���� �������� ���ư�����
	Capsule->BodyInstance.bNotifyRigidBodyCollision = true;	//projectile�� ���ؼ� �浹�Ϸ��� collision �Ѿ��Ѵ�
	Capsule->SetCollisionProfileName("BlockAll");	//���� �� �浹 

}

void ACArrow::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->OnComponentHit.AddDynamic(this, &ACArrow::OnComponentHit);

	Projectile->Deactivate();
	
}

void ACArrow::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/*
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
		
	if(!!weapon)
	{
		UCDoAction_Bow* bow = Cast<UCDoAction_Bow>(weapon->GetDoAction());
		
		if(!!bow)
		{
			SpawnRotation = bow->GetSpawnRotation();
			Forward = UKismetMathLibrary::GetForwardVector(SpawnRotation) * 2000;
		}
	}
	start = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right_Arrow");
	
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TEnumAsByte<EObjectTypeQuery> pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	objectTypes.Add(pawn);

	params.StartLocation = start;
	params.LaunchVelocity = Forward;
	params.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	params.TraceChannel = ECollisionChannel::ECC_Visibility;
	params.ObjectTypes = objectTypes;
	params.bTraceWithCollision = true;
	params.bTraceWithChannel = true;

	UGameplayStatics::PredictProjectilePath(GetWorld(), params, result);*/
}

void ACArrow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	bShoot = false;

	if (OnEndPlay.IsBound())
		OnEndPlay.Broadcast(this);
}


void ACArrow::Shoot(const FVector& InForward)
{
	Projectile->Velocity = InForward * Projectile->InitialSpeed ;
	
	Projectile->Activate();
	
	bShoot = true;

	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACArrow::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SetLifeSpan(LifeSpanAfterCollision);

	//�浹�Ѱ� Ignores�� �ִ°Ÿ� ����
	for (AActor* actor : Ignores)
		CheckTrue(actor == OtherActor);

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//hitMessage����
	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character && OnHit.IsBound())
		OnHit.Broadcast(this, character);
}

