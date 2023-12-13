#include "Weapons/AddOns/CNotify_Around.h"
#include "Global.h"

ACNotify_Around::ACNotify_Around()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Root);

}

void ACNotify_Around::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACNotify_Around::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	if (Negative == true)
	{
		CircleLocation -= Speed;

		if (CircleLocation <= 0)
			Negative = false;
	}

	if (Negative == false)
	{
		CircleLocation += Speed;
		if (CircleLocation >= Max)
		{
			Negative = true;
		}
	}

	FVector location = GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * CircleLocation);
	SetActorLocation(location);

}

FVector ACNotify_Around::GetNotifyLocation()
{
	return GetActorLocation();
}



