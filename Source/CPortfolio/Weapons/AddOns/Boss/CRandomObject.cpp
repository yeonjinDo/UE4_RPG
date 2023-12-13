#include "Weapons/AddOns/Boss/CRandomObject.h"
#include "Global.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Characters/CPlayer.h"
#include "Components/SphereComponent.h"

ACRandomObject::ACRandomObject()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere", Root);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara", Root);

	UNiagaraSystem* niagara;
	CHelpers::GetAsset<UNiagaraSystem>(&niagara, "NiagaraSystem'/Game/FX/sA_Megapack_v1/sA_StylizedAttacksPack/FX/NiagaraSystems/NS_AOE_Meteor_1.NS_AOE_Meteor_1'");
	Niagara->SetAsset(niagara);

	Sphere->SetCollisionProfileName("OverlapAllDynamic");
}

void ACRandomObject::BeginPlay()
{
	Super::BeginPlay();


	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACRandomObject::OnComponentBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACRandomObject::OnComponentEndOverlap);

	SetActorScale3D(Size);

	
	MaxTime = UKismetMathLibrary::RandomFloatInRange(0.8, 1.0);


	FTimerDelegate delegate = FTimerDelegate::CreateLambda([&]()
	{
		if (Hitted.Num() > 10)
			return;

		//중간에 빠질수도 있어서 거꾸로 돌려야한다
		for (int32 i = Hitted.Num() - 1; i >= 0; i--)
		{
			if(!!Attacker)
				HitData.SendDamage(Attacker, this, Hitted[i]);
		}
	});

	//DamageInterval뒤에 호출, Loop= true로 반복/ 즉, DamageInterval마다 호출되는 타이머
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, delegate, DamageInterval, true, 0);


}

void ACRandomObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckTrue(StopTick);

	CheckTrue(CurrentTime > MaxTime);
	CurrentTime += DeltaTime * Speed;
	

	FVector TrailLocation = FVector(
		Trail(PosA.X, PosB.X, PosC.X, PosD.X),
		Trail(PosA.Y, PosB.Y, PosC.Y, PosD.Y),
		Trail(PosA.Z, PosB.Z, PosC.Z, PosD.Z));

	if (TrailLocation == PosD)
		StopTick = true;

	SetActorLocation(TrailLocation);
	Sphere->SetWorldLocation(TrailLocation);
	
}

void ACRandomObject::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);
	
	ACEnemy_AI_Boss* boss = Cast<ACEnemy_AI_Boss>(character);
	CheckTrue(character == boss);

	Hitted.AddUnique(character);
}

void ACRandomObject::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);

	ACEnemy_AI_Boss* boss = Cast<ACEnemy_AI_Boss>(character);
	CheckTrue(character == boss); 

	Hitted.Remove(character);
}



void ACRandomObject::Init(FVector Start, FVector End)
{

	PosA = FVector(Start.X, Start.Y, 200);
	PosD = End;

	PosB = PosA + FVector(
		  DistanceFromStart * UKismetMathLibrary::RandomFloatInRange(-1.0,  1.0) * Start.RightVector
		+ DistanceFromStart * UKismetMathLibrary::RandomFloatInRange(-1, 1.0)    * Start.UpVector
		+ DistanceFromStart * UKismetMathLibrary::RandomFloatInRange(-1.0, 1)    * Start.ForwardVector
	);

	PosC = PosD + FVector(
		  DistanceFromEnd * UKismetMathLibrary::RandomFloatInRange(-1.0, 1.0) * End.RightVector
		+ DistanceFromEnd * UKismetMathLibrary::RandomFloatInRange(-1.0, 1.0) * End.UpVector
		+ DistanceFromEnd * UKismetMathLibrary::RandomFloatInRange( -1, 1.0)  * End.ForwardVector
	);

	
}

float ACRandomObject::Trail(float a, float b, float c, float d)
{
	float t = CurrentTime / MaxTime;
	
	return    FMath::Pow((1 - t), 3) * a
			+ FMath::Pow((1 - t), 2) * 3 * t * b
			+ FMath::Pow(t, 2) * 3 * (1 - t) * c
			+ FMath::Pow(t, 3) * d;
	
}

void ACRandomObject::SetAttacker(ACharacter* attacker)
{
	Attacker = attacker;
}
