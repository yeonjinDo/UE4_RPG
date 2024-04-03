#include "Weapons/AddOns/Boss/CBoss_Ray.h"
#include "Global.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Components/BoxComponent.h"

ACBoss_Ray::ACBoss_Ray()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	//CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara", Root);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Root);
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Root);

	//UNiagaraSystem* niagara;
	//CHelpers::GetAsset<UNiagaraSystem>(&niagara, "NiagaraSystem'/Game/FX/sA_Megapack_v1/sA_Rayvfx/Fx/NiagaraSystems/NS_Ray_1.NS_Ray_1'");
	//Niagara->SetAsset(niagara);

	
}

void ACBoss_Ray::BeginPlay()
{
	Super::BeginPlay();
	//Niagara->SetNiagaraVariableObject("Mesh_Scale", this);
	//Niagara->OnSystemFinished.AddDynamic(this, &ACBoss_Ray::OnSystemFinished);
	Particle->OnSystemFinished.AddDynamic(this, &ACBoss_Ray::OnSystemFinished);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACBoss_Ray::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACBoss_Ray::OnComponentEndOverlap);

	FTimerDelegate delegate = FTimerDelegate::CreateLambda([&]()
	{
		CheckFalse(Hitted.Num() > 0);

		//중간에 빠질수도 있어서 거꾸로 돌려야한다
		for (int32 i = Hitted.Num() - 1; i >= 0; i--)
		{
			HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[i]);
		}
	});

	//DamageInterval뒤에 호출, Loop= true로 반복/ 즉, DamageInterval마다 호출되는 타이머
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, delegate, DamageInterval, true, 0);

}

void ACBoss_Ray::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACBoss_Ray::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);

	ACEnemy_AI_Boss* boss = Cast<ACEnemy_AI_Boss>(OtherActor);
	CheckTrue(character == boss);

	Hitted.AddUnique(character);

}

void ACBoss_Ray::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
		Hitted.Remove(character);
}

void ACBoss_Ray::OnSystemFinished(UParticleSystemComponent* PSystem)
{
	Hitted.Empty();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	Destroy();
}