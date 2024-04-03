#include "Components/CTargetingComponent.h"
#include "Particles/ParticleSystem.h"
#include "CStateComponent.h"
#include "Global.h"
#include "Particles/ParticleSystemComponent.h"


UCTargetingComponent::UCTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());



}


void UCTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckNull(TargetCharacter);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(TargetCharacter);
	CheckNull(state);

	if(state->IsDeadMode() == false)
	{
		if (OwnerCharacter->GetDistanceTo(TargetCharacter) <= TraceDistance)
			TickTargeting();
		else
			EndTargeting();
	}

}

void UCTargetingComponent::BeginTargeting()
{
	FVector start = OwnerCharacter->GetActorLocation();
	FVector end   = OwnerCharacter->GetActorLocation();

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	TArray<FHitResult> hitResults;

	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, TraceDistance, "Targeting", 
									false, ignores, DebugType, hitResults, true);

	TArray<ACharacter*> targets;
	//for(int32 i = hitResults.Num(); i >= 0;  i--)
	for(int32 i = 0; i <= hitResults.Num() - 1; i++)
	{
		if (hitResults[i].GetActor()->GetClass() != OwnerCharacter->GetClass())
		{
			TargetCharacter = Cast<ACharacter>(hitResults[i].GetActor());

			targets.Add(TargetCharacter);
		}

	}

	ChangeTarget(CHelpers::GetNearlyToCameraFront(targets, OwnerCharacter));

}

void UCTargetingComponent::EndTargeting()
{
	TargetCharacter = nullptr;
	if (!!Particle)
		Particle->DestroyComponent();

}

void UCTargetingComponent::ChangeTarget(ACharacter* InCandidate)
{
	if (InCandidate == nullptr)
		EndTargeting();

	if(!!InCandidate)
	{
		if (!!Particle)
			Particle->DestroyComponent();

		FTransform transform;
		//CHelpers::PlayEffect(GetWorld(), ParticleAsset, transform, InCandidate->GetMesh(), "Targeting");

		Particle = UGameplayStatics::SpawnEmitterAttached(ParticleAsset, InCandidate->GetMesh(), "Targeting", FVector(0, 0, -10));

		TargetCharacter = InCandidate;
	}

}

void UCTargetingComponent::TickTargeting()
{
	FRotator ownerToTarget;
	FRotator controlRotation;

	controlRotation = OwnerCharacter->GetControlRotation();

	FVector start = OwnerCharacter->GetActorLocation();
	FVector end   = TargetCharacter->GetActorLocation();

	FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(start, end);

	ownerToTarget = UKismetMathLibrary::MakeRotator(lookAtRotation.Roll, controlRotation.Pitch, lookAtRotation.Yaw);


	if(UKismetMathLibrary::EqualEqual_RotatorRotator(controlRotation, ownerToTarget, FinishAngle))
	{
		OwnerCharacter->GetController()->SetControlRotation(ownerToTarget);

		if (MovingFocus)
			MovingFocus = false;
	}
	else
	{
		FRotator targetRotator = UKismetMathLibrary::MakeRotator(ownerToTarget.Roll, controlRotation.Pitch, ownerToTarget.Yaw);
		FRotator newRotation   = UKismetMathLibrary::RInterpTo(controlRotation, targetRotator, GetWorld()->GetDeltaSeconds(), InterpSpeed);

		OwnerCharacter->GetController()->SetControlRotation(newRotation);
	}

}

void UCTargetingComponent::ChangeFocus(bool bInRight)
{
	CheckNull(TargetCharacter);

	TMap<float, ACharacter*> map;
	float key;
	float minimum = 999999;
	


	if (MovingFocus == false)
	{
		MovingFocus = true;

		FVector start = OwnerCharacter->GetActorLocation();
		FVector end   = OwnerCharacter->GetActorLocation();

		TArray<AActor*> ignores;
		ignores.Add(OwnerCharacter);
		ignores.Add(TargetCharacter);

		TArray<FHitResult> hitResults;

		UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, TraceDistance, "Targeting", false, ignores, DebugType, hitResults, true);

		for (int32 i = 0; i <= hitResults.Num() - 1; i++)
		{
			AActor* hit = hitResults[i].GetActor();

			ACharacter* hitCharacter = Cast<ACharacter>(hit);

			FVector direction = (hit->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal2D(1e-04);
			FVector forward   = UKismetMathLibrary::GetForwardVector(OwnerCharacter->GetControlRotation());
			FVector up        = OwnerCharacter->GetActorUpVector();

			FVector cross = UKismetMathLibrary::Cross_VectorVector(forward, direction);
			float   dot   = UKismetMathLibrary::Dot_VectorVector(cross, up);

			map.Add(dot, hitCharacter);

		}

		ACharacter* candidate = nullptr;
		for(TTuple<float, ACharacter*> keys : map)
		{
			key = keys.Key;

			if ((bInRight && key > 0) || (!bInRight && key < 0))
			{
				float absKey = UKismetMathLibrary::Abs(key);

				if( absKey < minimum)
				{
					minimum = absKey;
					candidate = *map.Find(key);
				}
			}
		}

		ChangeTarget(candidate);

		 
	}
}

void UCTargetingComponent::ToggleTarget()
{

	if (TargetCharacter == nullptr)
		BeginTargeting();

	else if (!!TargetCharacter)
		EndTargeting();

}

void UCTargetingComponent::ChangeTargetLeft()
{
	ChangeFocus(false);
}

void UCTargetingComponent::ChangeTargetRight()
{
	ChangeFocus(true);
}

