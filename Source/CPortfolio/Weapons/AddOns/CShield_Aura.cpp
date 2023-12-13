#include "Weapons/AddOns/CShield_Aura.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"

ACShield_Aura::ACShield_Aura()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Capsule);


	Capsule->SetCapsuleHalfHeight(150);
	Capsule->SetCapsuleRadius(150);



}

void ACShield_Aura::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(5);

}

void ACShield_Aura::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ACShield_Aura::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	SetActorLocation(GetOwner()->GetActorLocation());
	SetActorRotation(FRotator(0, 0, 0));
}
