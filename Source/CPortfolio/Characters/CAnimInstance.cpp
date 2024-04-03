#include "Characters/CAnimInstance.h"
#include "Global.h"
#include "Components/CMontagesComponent.h"
#include "Components/CMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/CSubAction.h"
#include "Parkour/CParkourComponent.h"


void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	if (!!Weapon)
		Weapon->OnWeaponTypeChange.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);

}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();

	FRotator rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rotator2 = OwnerCharacter->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator2);
	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);
	Direction = PrevRotation.Yaw;

	Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);
	bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();

	

	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(OwnerCharacter);
	UCFeetComponent*    feet    = CHelpers::GetComponent<UCFeetComponent>(OwnerCharacter);


	CheckNull(Weapon);

	//Bow- aiming
	bAim_Bow = true;
	if (!!Weapon->GetSubAction())
	{
		bAim_Bow &= (WeaponType == EWeaponType::Bow);
		bAim_Bow &= (Weapon->GetSubAction()->GetInAction());
	}

	//UnarmedMode일때만 IK 수행
	CheckFalse(Weapon->IsUnarmedMode());

	//초기화
	bFeet = false;

	//IK수행 조건
	if(!!parkour && !!feet)
	{
		bFeet = parkour->IsExecuting() == false;
		FeetData = feet->GetData();
	}
	else if(!!feet)
	{
		bFeet = true;
		FeetData = feet->GetData();
	}


	
}


void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}
