#include "Weapons/AddOns/CNotify_Around.h"
#include "Global.h"
#include "Components/DecalComponent.h"

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
	SetActorRotation(FRotator(0, -90, 0));

	if (GetGroundLocation(location))
	{
		location.Z += 10;

		SetActorLocation(location);
	}
}

bool ACNotify_Around::GetGroundLocation(FVector& OutLocation)
{
	TArray<AActor*> ignores;
	ignores.Add(GetOwner());

	FHitResult hitResult;
	FVector start = FVector(GetActorLocation().X, GetActorLocation().Y, GetOwner()->GetActorLocation().Z + 180);
	FVector end = FVector(GetActorLocation().X, GetActorLocation().Y, GetOwner()->GetActorLocation().Z - 180);

	UKismetSystemLibrary::LineTraceSingle(GetOwner()->GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1,
		true, ignores, EDrawDebugTrace::None, hitResult, true, FLinearColor::Green, FLinearColor::Red);
	 
	CheckFalseResult(hitResult.bBlockingHit, false);
	
	OutLocation = hitResult.Location;

	return true;
}

FVector ACNotify_Around::GetNotifyLocation()
{
	return GetActorLocation();
}

FRotator ACNotify_Around::GetNotifyRotation()
{
	return GetActorRotation();
}



