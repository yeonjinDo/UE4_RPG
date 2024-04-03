#include "Components/CZoomComponent.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"


UCZoomComponent::UCZoomComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCZoomComponent::BeginPlay()
{
	Super::BeginPlay();

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(GetOwner());
	CheckNull(SpringArm);

	CurrentValue = SpringArm->TargetArmLength;
	CurrentHeight = SpringArm->GetRelativeLocation().Z;
}


void UCZoomComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckNull(SpringArm);

	CheckTrue(UKismetMathLibrary::NearlyEqual_FloatFloat(SpringArm->TargetArmLength, CurrentValue));
	SpringArm->TargetArmLength = UKismetMathLibrary::FInterpTo(SpringArm->TargetArmLength, CurrentValue, DeltaTime, InterpSpeed);

	float height;
	height = UKismetMathLibrary::FInterpTo(SpringArm->GetRelativeLocation().Z, CurrentHeight, DeltaTime, InterpSpeed);

	SpringArm->SetRelativeLocation(FVector(SpringArm->GetRelativeLocation().X, SpringArm->GetRelativeLocation().Y, height));

}

void UCZoomComponent::SetZoomValue(float InValue)
{
	CurrentValue += (Speed * InValue);
	CurrentValue = FMath::Clamp(CurrentValue, Range.X, Range.Y);

	CurrentHeight += (HeightSpeed * InValue);
	CurrentHeight = FMath::Clamp(CurrentHeight, HeightRange.X, HeightRange.Y);

}