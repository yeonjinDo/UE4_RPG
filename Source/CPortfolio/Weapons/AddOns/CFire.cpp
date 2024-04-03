#include "Weapons/AddOns/CFire.h"
#include "Global.h"
#include "Components/BoxComponent.h"

ACFire::ACFire()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Root);
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Particle);

}

void ACFire::BeginPlay()
{
	Super::BeginPlay();

	
	//충돌 연결
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACFire::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACFire::OnComponentEndOverlap);

	Particle->OnSystemFinished.AddDynamic(this, &ACFire::OnSystemFinished);

	FTimerDelegate delegate = FTimerDelegate::CreateLambda([&]()
	{	//중간에 빠질수도 있어서 거꾸로 돌려야한다
		for (int32 i = Hitted.Num() - 1; i >= 0; i--)
			HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[i]);
	});

	GetOwner()->GetWorld()->GetTimerManager().SetTimer(TimerHandle, delegate, DamageInterval, true, 0);

}

void ACFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACFire::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
		Hitted.AddUnique(character);
}

void ACFire::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
		Hitted.Remove(character);
}


void ACFire::OnSystemFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}