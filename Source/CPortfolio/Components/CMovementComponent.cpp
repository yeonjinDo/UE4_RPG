#include "Components/CMovementComponent.h"

#include "CMontagesComponent.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UCMovementComponent::UCMovementComponent()
{
}


void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

}

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)InType];
}

void UCMovementComponent::OnSprint()
{
	SetSpeed(ESpeedType::Sprint);
}

void UCMovementComponent::OnRun()
{
	SetSpeed(ESpeedType::Run);
}

void UCMovementComponent::OnWalk()
{
	SetSpeed(ESpeedType::Walk);
}

void UCMovementComponent::OnDash(UNiagaraSystem* DashEffect)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 2500;
	OwnerCharacter->GetCharacterMovement()->GroundFriction = 10;
	OwnerCharacter->GetCharacterMovement()->BrakingDecelerationWalking = 0;
	OwnerCharacter->GetCharacterMovement()->MaxAcceleration = 5000;

	FTransform transform = OwnerCharacter->GetTransform();
	FVector  location = transform.GetLocation();
	FRotator rotation = FRotator(transform.GetRotation());
	FVector     scale = transform.GetScale3D();

	if(!!DashEffect)
	{
		Dash = UNiagaraFunctionLibrary::SpawnSystemAttached(DashEffect, OwnerCharacter->GetMesh(), "Back_Shield", location, rotation,
			scale, EAttachLocation::KeepWorldPosition, true, ENCPoolMethod::None);
	}

	//Visibility
	OwnerCharacter->GetMesh()->SetVisibility(false);

}

void UCMovementComponent::OnEndDash()
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 200;
	OwnerCharacter->GetCharacterMovement()->BrakingDecelerationWalking = 2048;
	OwnerCharacter->GetCharacterMovement()->GroundFriction = 5;
	OwnerCharacter->GetCharacterMovement()->MaxAcceleration = 1000;

	if(!!Dash)
	{
		Dash->Deactivate();
	}

	//Visibility
	OwnerCharacter->GetMesh()->SetVisibility(true);
	
}

void UCMovementComponent::EnableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCMovementComponent::DisableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}



void UCMovementComponent::OnMoveForward(float InAxis)
{
	CheckFalse(bCanMove);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	//절대방향 x축으로 방향 바꿔준다 Warp-SubAction에서의 캐릭터 움직임 방향 처리
	if (bTopViewCamera)
		direction = FVector::XAxisVector;

	
	OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UCMovementComponent::OnMoveRight(float InAxis)
{
	CheckFalse(bCanMove);

	FRotator rotator  = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	if (bTopViewCamera)
		direction = FVector::YAxisVector;

	OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UCMovementComponent::OnHorizontalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerYawInput(InAxis * HorizontalLook * GetWorld()->GetDeltaSeconds());

	//InAxis를 곱하는 이유 : 감도 조절
/*원래 horizontalLook만 들어가는데 InAxis *  GetWorld()->GetDeltaSeconds()를 넣어서 부드럽게 유지되도록! */
}

void UCMovementComponent::OnVerticalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerPitchInput(InAxis * VerticalLook * GetWorld()->GetDeltaSeconds());
}

FVector UCMovementComponent::GetFutureLocation(ACharacter* InCharacter, float InTime)
{
	//캐릭터의 현재 위치와 속도를 계산하여 InTime뒤의 캐릭터의 위치를 예상
	FVector velocity = InCharacter->GetVelocity();
	FVector location = InCharacter->GetActorLocation();

	FVector distance = velocity * FVector(1, 1, 0) * InTime;

	return distance + location;	//InTime동안 움직일 거리 + 현재위치
}
 