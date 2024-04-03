#include "Components/CAirComboComponent.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Global.h"


UCAirComboComponent::UCAirComboComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCAirComboComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
}


void UCAirComboComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UCAirComboComponent::Launch(ACharacter* InCharacter)
{
	CheckNull(InCharacter);

	CheckTrue(bInAir);

	InCharacter->LaunchCharacter(LaunchVelocity, false, false);

	bInAir = true;
}

void UCAirComboComponent::Fall(ACharacter* InCharacter)
{
	CheckNull(InCharacter);
	
	InCharacter->LaunchCharacter(FallVelocity, false, false);
	//InCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
}

