#include "Components/CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
//#define LOG_UCFeetComponent

UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float    leftDistance, rightDistance;
	FRotator leftRotation, rightRotation;

	Trace(LeftSocket,  leftDistance,  leftRotation);
	Trace(RightSocket, rightDistance, rightRotation);


	float offset = FMath::Min(leftDistance, rightDistance);
	//허리
	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);
	//왼발, 오른발(이전 프레임~현재 프레임)
	Data.LeftDistance.X   = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - offset), DeltaTime, InterpSpeed);
	Data.RightDistance.X  = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(rightDistance - offset), DeltaTime, InterpSpeed);
	//발 회전
	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterpSpeed);


#ifdef LOG_UCFeetComponent
	CLog::Print(Data.LeftDistance,   11);
	CLog::Print(Data.RightDistance,  12);
	CLog::Print(Data.PelvisDistance, 13);
	CLog::Print(Data.LeftRotation,   14);
	CLog::Print(Data.RightRotation,  15);
#endif


}

void UCFeetComponent::Trace(FName InName, float& OutDistance, FRotator& OutRotation)
{
	FVector socket = OwnerCharacter->GetMesh()->GetSocketLocation(InName);	//World/ x, y만 사용

	float z = OwnerCharacter->GetActorLocation().Z;	//캐릭터 중심 z값
	FVector start = FVector(socket.X, socket.Y, z);

	z = start.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;
	FVector end   = FVector(socket.X, socket.Y, z);


	//lineTrace
	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1,
		true, ignores, DrawDebug, hitResult, true, FLinearColor::Green, FLinearColor::Red);


	//초기화
	OutDistance = 0;
	OutRotation = FRotator::ZeroRotator;

	CheckFalse(hitResult.bBlockingHit);


	float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();
	OutDistance = length + OffsetDistance - TraceDistance;

	//발 각도
	float roll  = +UKismetMathLibrary::DegAtan2(hitResult.Normal.Y, hitResult.Normal.Z);	   //y->z atan으로 cos값을 알아내기 위함
	float pitch = -UKismetMathLibrary::DegAtan2(hitResult.Normal.X, hitResult.Normal.Z);
	
	OutRotation = FRotator(pitch, 0, roll);


}

