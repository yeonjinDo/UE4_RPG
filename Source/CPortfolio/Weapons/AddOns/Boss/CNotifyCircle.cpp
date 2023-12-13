#include "Weapons/AddOns/Boss/CNotifyCircle.h"
#include "Global.h"
#include "Components/BoxComponent.h"

ACNotifyCircle::ACNotifyCircle()
{
	
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Root);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Root);

	Box->SetCollisionProfileName("NoCollision");

}

void ACNotifyCircle::BeginPlay()
{
	Super::BeginPlay();

	SetActorScale3D(CircleSize);
	//SetLifeSpan(3);
}


