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

	//������� x������ ���� �ٲ��ش� Warp-SubAction������ ĳ���� ������ ���� ó��
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

	//InAxis�� ���ϴ� ���� : ���� ����
/*���� horizontalLook�� ���µ� InAxis *  GetWorld()->GetDeltaSeconds()�� �־ �ε巴�� �����ǵ���! */
}

void UCMovementComponent::OnVerticalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerPitchInput(InAxis * VerticalLook * GetWorld()->GetDeltaSeconds());
}

FVector UCMovementComponent::GetFutureLocation(ACharacter* InCharacter, float InTime)
{
	//ĳ������ ���� ��ġ�� �ӵ��� ����Ͽ� InTime���� ĳ������ ��ġ�� ����
	FVector velocity = InCharacter->GetVelocity();
	FVector location = InCharacter->GetActorLocation();

	FVector distance = velocity * FVector(1, 1, 0) * InTime;

	return distance + location;	//InTime���� ������ �Ÿ� + ������ġ
}
 