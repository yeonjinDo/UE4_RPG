#include "Weapons/AddOns/Boss/CSpinHammer.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


ACSpinHammer::ACSpinHammer()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &RotationPoint, "RotationPoint");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Skeletal, "SkeletalMesh", RotationPoint);
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Skeletal);
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Weapons/GreateHammer/SM_GreatHammer_SM_GreatHammer.SM_GreatHammer_SM_GreatHammer'");
	Skeletal->SetSkeletalMesh(mesh);
}

void ACSpinHammer::BeginPlay()
{
	Super::BeginPlay();

	Projectile->Activate();

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACSpinHammer::OnComponentBeginOverlap);

	Spin();
	
}

void ACSpinHammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);

}

void ACSpinHammer::Spin()
{
	FOnTimelineFloat timeline;
	timeline.BindUFunction(this, "OnSpinning");

	Timeline.AddInterpFloat(Curve, timeline);
	Timeline.SetPlayRate(SpinSpeed);

	FOnTimelineEvent timelineEvent;
	timelineEvent.BindUFunction(this, "OnTimeLineFinished");

	Timeline.SetTimelineFinishedFunc(timelineEvent);

	OriginRotation = RotationPoint->GetComponentTransform().GetRotation();
	Timeline.PlayFromStart();
} 

void ACSpinHammer::OnSpinning(float Output)
{
	FRotator rot = FRotator(OriginRotation.Y, Output, OriginRotation.X);

	RotationPoint->SetWorldRotation(rot);
}

void ACSpinHammer::OnTimeLineFinished()
{
	Timeline.PlayFromStart();
}

void ACSpinHammer::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (!!character)
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(CameraShakeClass);

		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, character);
		Destroy();
	}
	else
		Destroy();
}

