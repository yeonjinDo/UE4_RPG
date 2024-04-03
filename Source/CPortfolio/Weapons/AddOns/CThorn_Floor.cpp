#include "Weapons/AddOns/CThorn_Floor.h"
#include "Global.h"
#include "Components/BoxComponent.h"


ACThorn_Floor::ACThorn_Floor()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle");

}

void ACThorn_Floor::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(4);

	Box->SetRelativeScale3D(FVector(9, 9, 1));
	Box->SetWorldLocation(GetActorLocation() - 50);
	
	
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACThorn_Floor::OnComponentBeginOverlap);
	
	SetActorRotation(GetOwner()->GetActorRotation() + FRotator(0, 120, 0));
}


void ACThorn_Floor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (cnt < 50)
	{
		cnt ++;
		Box->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + cnt));
	}

}

void ACThorn_Floor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (!!character)
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, character);
}
