#include "Weapons/AddOns/Boss/CNotifyBox.h"
#include "Global.h"
#include "Components/BoxComponent.h"

ACNotifyBox::ACNotifyBox()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Cube, "Cube", Root);


}

void ACNotifyBox::BeginPlay()
{
	Super::BeginPlay();

	Cube->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetActorScale3D(BoxSize);
	SetLifeSpan(3);
}

void ACNotifyBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

