#include "Characters/CEnemy_Human.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CAirComboComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMontagesComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACEnemy_Human::ACEnemy_Human()
{
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/ABP_Character.ABP_Character_C'");
	GetMesh()->SetAnimClass(animInstance);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
}

void ACEnemy_Human::BeginPlay()
{
	Super::BeginPlay();
	 
	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy_Human::OnStateTypeChanged);
	SubState->OnSubStateTypeChanged.AddDynamic(this, &ACEnemy_Human::OnSubStateTypeChanged);

}

void ACEnemy_Human::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	CheckFalse(SubState->IsAirComboMode());


	if (bCheck == false)
	{
		height = GetActorLocation().Z;
		bCheck = true;
	}

	IICharacter::LimitHeight(this, height);

}

void ACEnemy_Human::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hitted:	Hitted();   break;
	case EStateType::Avoid:		Avoid();    break;
	case EStateType::Dead:		Dead();     break;
	case EStateType::Rise:		Rise();     break;
	case EStateType::Parrying:	Parrying(); break;
	}
}


void ACEnemy_Human::OnSubStateTypeChanged(ESubStateType InPrevType, ESubStateType InNewType)
{
	switch (InNewType)
	{
	case ESubStateType::AirCombo:	AirComboLaunch();   break;

	}
}
  
void ACEnemy_Human::Landed(const FHitResult& HitResult)
{
	AirCombo->InAir(false);
	bCheck = false;

}

void ACEnemy_Human::Rise()
{
	Montages->PlayRiseMode();
}

void ACEnemy_Human::Avoid()
{
	Montages->PlayAvoidMode();
}

void ACEnemy_Human::AirComboLaunch()
{
	AirCombo->Launch(this);

	GetCharacterMovement()->GravityScale = 0;

	Behavior->SetWaitMode();
}

void ACEnemy_Human::Parrying()
{
	Montages->PlayParryingMode();

	Behavior->SetParryingMode();
}
