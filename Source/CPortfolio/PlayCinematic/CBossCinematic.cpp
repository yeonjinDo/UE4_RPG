#include "PlayCinematic/CBossCinematic.h"
#include "Global.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Components/BoxComponent.h"


ACBossCinematic::ACBossCinematic()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle");

}

void ACBossCinematic::BeginPlay()
{
	Super::BeginPlay();

	//SetLifeSpan(4);

	Box->SetRelativeScale3D(FVector(9, 9, 1));
	Box->SetWorldLocation(GetActorLocation() - 50);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACBossCinematic::OnComponentBeginOverlap);

	
}

void ACBossCinematic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACBossCinematic::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*FMovieSceneSequencePlaybackSettings settings;

	ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Sequence, settings)->Play();*/

}
