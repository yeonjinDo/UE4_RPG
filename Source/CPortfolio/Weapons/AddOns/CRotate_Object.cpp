#include "Weapons/AddOns/CRotate_Object.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"


ACRotate_Object::ACRotate_Object()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Capsule);

	//캡슐을 구 모양으로 세팅
	Capsule->SetCapsuleHalfHeight(44);
	Capsule->SetCapsuleRadius(44);

	//살아있을 시간
	InitialLifeSpan = 5;
	
	//HitData를 C에서 직접 설정해준 것 launch, power, montage 설정
	HitData.Launch = 0;
	HitData.Power = 5;
	CHelpers::GetAsset<UAnimMontage>(&HitData.Montage, "AnimMontage'/Game/Character/Montages/Common/HitReaction_Montage.HitReaction_Montage'");


}

void ACRotate_Object::BeginPlay()
{
	Super::BeginPlay();

	//random으로 시작
	Angle = UKismetMathLibrary::RandomFloatInRange(0, 360);


	//충돌 연결
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACRotate_Object::OnComponentBeginOverlap);
	Capsule->OnComponentEndOverlap.AddDynamic(this, &ACRotate_Object::OnComponentEndOverlap);


	//타이머 시작
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACRotate_Object::SendDamage, DamageInterval, true);


}

void ACRotate_Object::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector location = GetOwner()->GetActorLocation();

	Angle += (bNegative ? -Speed : +Speed) * DeltaTime;

	//Angle 초기화 안하면 짐벌락 발생 가능성이 생긴다
	if (FMath::IsNearlyEqual(Angle, bNegative ? -360 : +360))
		Angle = 0;	

	FVector distance = FVector(Distance, 0, 0);
	FVector value = distance.RotateAngleAxis(Angle, FVector::UpVector);		//yaw회전
	location += value;

	SetActorLocation(location);
	SetActorRotation(FRotator(0, Angle, 0));


}

void ACRotate_Object::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//타이머 제거
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void ACRotate_Object::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
		Hitted.AddUnique(character);
}

void ACRotate_Object::OnComponentEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
		Hitted.Remove(character);
}

void ACRotate_Object::SendDamage()
{
	for (int32 i = Hitted.Num() - 1; i >= 0; i--)
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[i]);
}

