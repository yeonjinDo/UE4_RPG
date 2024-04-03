#include "Characters/CPlayer.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "CEnemy.h"
#include "CEnemy_Human.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CAirComboComponent.h"
#include "Components/CDefendComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CSubStateComponent.h"
#include "Components/CTargetingComponent.h"
#include "Components/CZoomComponent.h"
#include "Components/WidgetComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CWeaponStructures.h"
#include "Widgets/CUserWidget_PlayerStatus.h"
#include "Widgets/CUserWidget_SkillButton.h"
#include "Widgets/CUserWidget_WeaponMenu.h"
#include "Widgets/CUserWidget_SkillMenu.h"


ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;


	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCSubStateComponent>(this, &SubState, "SubState");
	CHelpers::CreateActorComponent<UCParkourComponent>(this, &Parkour, "Parkour");
	CHelpers::CreateActorComponent<UCZoomComponent>(this, &Zoom, "Zoom");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCDefendComponent>(this, &Defend, "Defend");
	CHelpers::CreateActorComponent<UCTargetingComponent>(this, &Targeting, "Targeting");
	CHelpers::CreateActorComponent<UCAirComboComponent>(this, &AirCombo, "AirCombo");
	CHelpers::CreateActorComponent<UNiagaraComponent>(this, &Niagara, "Niagara");

	CHelpers::CreateActorComponent<UAnimMontage>(this, &TestMontage, "TestMontage");



	//플레이어 메시 위치 조정
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	//메시 설정
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);


	////애니메이션 블루프린트 설정
	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/ABP_Character.ABP_Character_C'");
	GetMesh()->SetAnimClass(animInstance);

	//카메라 붙일 위치 설정
	SpringArm->SetRelativeLocation(FVector(0, 0, 200));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 600;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	//플레이어가 걷다가 멈출때 갑자기 멈추지 않고 서서히 멈추도록 조정
	GetCharacterMovement()->BrakingDecelerationWalking = 2048;
	GetCharacterMovement()->GroundFriction = 8;
	GetCharacterMovement()->MaxAcceleration = 500;

	//parkour arrow그룹 컴포넌트 생성 
	CHelpers::CreateComponent<USceneComponent>(this, &ArrowGroup, "ArrowGroup", GetCapsuleComponent());
	for (int32 i = 0; i < (int32)EParkourArrowType::Max; i++)
	{
	
		FString name = StaticEnum<EParkourArrowType>()->GetNameStringByIndex(i);
		CHelpers::CreateComponent<UArrowComponent>(this, &Arrows[i], FName(name), ArrowGroup);

		//화살표 종류별로 생성
		switch ((EParkourArrowType)i)
		{
		case EParkourArrowType::Center:
			Arrows[i]->ArrowColor = FColor::Red;
			break;

		case EParkourArrowType::Ceil:
			Arrows[i]->ArrowColor = FColor::Green;
			Arrows[i]->SetRelativeLocation(FVector(0, 0, 100));
			break;

		case EParkourArrowType::Floor:
			Arrows[i]->ArrowColor = FColor::Blue;
			Arrows[i]->SetRelativeLocation(FVector(0, 0, -80));
			break;

		case EParkourArrowType::Left:
			Arrows[i]->ArrowColor = FColor::Magenta;
			Arrows[i]->SetRelativeLocation(FVector(0, -30, 0));
			break;

		case EParkourArrowType::Right:
			Arrows[i]->ArrowColor = FColor::Magenta;
			Arrows[i]->SetRelativeLocation(FVector(0, 30, 0));
			break;

		case EParkourArrowType::Land:
			Arrows[i]->ArrowColor = FColor::Yellow;
			Arrows[i]->SetRelativeLocation(FVector(200, 0, 100));
			Arrows[i]->SetRelativeRotation(FRotator(-90, 0, 0));
			break;

		}

	}


#if WITH_EDITOR

	CHelpers::GetClass<UCUserWidget_PlayerStatus>(&HpBar, "WidgetBlueprint'/Game/Widgets/WB_PlayerStatus.WB_PlayerStatus_C'");
	CHelpers::GetClass<UCUserWidget_WeaponMenu>(&Menu, "WidgetBlueprint'/Game/Widgets/WB_WeaponMenu.WB_WeaponMenu_C'");
	CHelpers::GetClass<UCUserWidget_SkillMenu>(&SkillMenu, "WidgetBlueprint'/Game/Widgets/WB_SkillMenu.WB_SkillMenu_C'");

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	Widget = CreateWidget(controller, HpBar);

	if (!!Widget)
		Widget->AddToViewport();

	SkillWidget = CreateWidget(controller, SkillMenu);

	if (!!SkillWidget)
		SkillWidget->AddToViewport();


	UUserWidget* menuWidget = CreateWidget(controller, Menu);
	weaponMenu = Cast<UCUserWidget_WeaponMenu>(menuWidget);

	if(!!weaponMenu)
	{
		weaponMenu->AddToViewport();
		weaponMenu->SetVisibility(ESlateVisibility::Hidden);
	}

#endif

}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	Movement->SetSpeed(ESpeedType::Run);
	//Movement->DisableControlRotation();	//사방으로 뛰도록
	Movement->EnableControlRotation();	

	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);
	SubState->OnSubStateTypeChanged.AddDynamic(this, &ACPlayer::OnSubStateTypeChanged);


}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!!Status)
		Status->RecoverMana(0.05f);

	if (SubState->IsAirComboMode())
	{
		
		if (bCheck == false)
		{
			height = GetActorLocation().Z;
			bCheck = true;
		}

		IICharacter::LimitHeight(this, height);
		AirComboLaunch();
	}

	//패링상태에서 처형키 입력시 처형 ~ 
	if (bExecute == true && State->IsParryingMode())
	{
		UCMontagesComponent* enemyMontage = CHelpers::GetComponent<UCMontagesComponent>(enemy);

		Montages->PlayExecuteMode();
		enemyMontage->PlayExecuteMode();

	}

	
#if WITH_EDITOR

	UCUserWidget_PlayerStatus* hpBar = Cast<UCUserWidget_PlayerStatus>(Widget);
	if(!!hpBar)
	{
		hpBar->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
		hpBar->UpdateMana(Status->GetMana(), Status->GetMaxMana());
	}
	
#endif

}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",    Movement, &UCMovementComponent::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight",      Movement, &UCMovementComponent::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", Movement, &UCMovementComponent::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook",   Movement, &UCMovementComponent::OnVerticalLook);

	PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::SetZooming);

	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed,  Movement, &UCMovementComponent::OnSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnRun);

	PlayerInputComponent->BindAction("Avoid",  EInputEvent::IE_Pressed, this, &ACPlayer::OnAvoid);

	PlayerInputComponent->BindAction("Defend", EInputEvent::IE_Pressed, Defend, &UCDefendComponent::OnDefend);
	PlayerInputComponent->BindAction("Defend", EInputEvent::IE_Released,Defend, &UCDefendComponent::EndDefend);

	PlayerInputComponent->BindAction("Target_Left",  EInputEvent::IE_Pressed, Targeting, &UCTargetingComponent::ChangeTargetLeft);
	PlayerInputComponent->BindAction("Target",       EInputEvent::IE_Pressed, Targeting, &UCTargetingComponent::ToggleTarget);
	PlayerInputComponent->BindAction("Target_Right", EInputEvent::IE_Pressed, Targeting, &UCTargetingComponent::ChangeTargetRight);

	PlayerInputComponent->BindAction("Dash", EInputEvent::IE_Pressed, this,  &ACPlayer::Dash);
	PlayerInputComponent->BindAction("Dash", EInputEvent::IE_Released, this, &ACPlayer::End_Dash);

	PlayerInputComponent->BindAction("Fist",   EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetFistMode);
	PlayerInputComponent->BindAction("Sword",  EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetSwordMode);
	PlayerInputComponent->BindAction("Hammer", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetHammerMode);
	PlayerInputComponent->BindAction("Warp",   EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetWarpMode);
	PlayerInputComponent->BindAction("Around", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetAroundMode);
	PlayerInputComponent->BindAction("Bow",    EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetBowMode);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);

	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Pressed,  this, &ACPlayer::On_RightButton);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Released, this, &ACPlayer::Off_RightButton);

	PlayerInputComponent->BindAction("QSkill",    EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SkillAction_Pressed);

	PlayerInputComponent->BindAction("AirMode",   EInputEvent::IE_Pressed, this, &ACPlayer::AirMode);
	PlayerInputComponent->BindAction("AirMode",   EInputEvent::IE_Released,this, &ACPlayer::End_AirMode);

	PlayerInputComponent->BindAction("Menu",   EInputEvent::IE_Pressed,this, &ACPlayer::ShowMenu);
	PlayerInputComponent->BindAction("Menu",   EInputEvent::IE_Released,this, &ACPlayer::HideMenu);

	PlayerInputComponent->BindAction("Execution", EInputEvent::IE_Pressed, this, &ACPlayer::Execute);
	PlayerInputComponent->BindAction("Execution", EInputEvent::IE_Released, this, &ACPlayer::End_Hitted);

	PlayerInputComponent->BindAction("Jump",   EInputEvent::IE_Pressed,this, &ACPlayer::Jump);
	PlayerInputComponent->BindAction("Test",   EInputEvent::IE_Pressed,this, &ACPlayer::Test);

}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::BackStep:    Roll();	        break;
	case EStateType::Avoid:		  Avoid();	            break;
	case EStateType::Defend:      Defend->Defend(); 	break;
	case EStateType::Parrying:    Defend->Parrying();	break;
	case EStateType::Hitted:	  Hitted();             break;
	case EStateType::Rise:		  Rise();               break;
	}
}

void ACPlayer::OnSubStateTypeChanged(ESubStateType InPrevType, ESubStateType InNewType)
{
	switch (InNewType)
	{
	case ESubStateType::Faint:    Faint();	        break;
	}
}

void ACPlayer::On_RightButton()
{	//unarmedmode에서 subaction = parkour
	if (Weapon->IsUnarmedMode())
	{
		Parkour->DoParkour();
		
		return;
	}

	//zoomcomponent에서 tick이 계속 실행되어 bow에서 발생하는 버그 해결
	if (Weapon->IsBowMode())
		Zoom->SetComponentTickEnabled(false);

	Weapon->SubAction_Pressed();
}

void ACPlayer::Off_RightButton()
{
	if (Weapon->IsBowMode())
		Zoom->SetComponentTickEnabled(true);


	Weapon->SubAction_Released();
}

void ACPlayer::OnAvoid()
{
	CheckTrue(InputComponent->GetAxisValue("MoveForward") > 0.0f);

	if (State->IsDefendMode())
		State->SetAvoidMode();

	CheckFalse(State->IsIdleMode());
	CheckFalse(Movement->CanMove());

	State->SetBackStepMode();
}

void ACPlayer::Rise()
{
	Montages->PlayRiseMode();
	Movement->Move();
	SubState->SetGroundMode();
}

void ACPlayer::Test()
{
	CLog::Log("Test");
	//PlayAnimMontage(TestMontage);
}

void ACPlayer::Faint()
{
	Movement->Stop();
	//Movement->EnableControlRotation();

}

void ACPlayer::Jump()
{
	CheckTrue(InputComponent->GetAxisValue("MoveForward") <= 0.0f);

	if (JumpCount < 2)
	{
		LaunchCharacter(FVector(0, 0, 500), false, false);

		if (State->IsJumpMode())
		{
			Montages->PlayDoubleJumpMode();
		}
		else
		{
			State->SetJumpMode();
			Montages->PlayJumpMode();
		}

		JumpCount++;

	}
	else
	{
		return;
	}
	

}

void ACPlayer::Dash()
{
	SubState->SetDashMode();

	//Movement
	Movement->OnDash(DashEffect);

	//Visibility
	if (Weapon->OnWeaponHidden.IsBound())
		Weapon->OnWeaponHidden.Broadcast();
}

void ACPlayer::End_Dash()
{
	SubState->SetGroundMode();
	
	//Movement
	Movement->OnEndDash();
	Movement->OnSprint();

	//Visibility
	if (Weapon->OnWeaponVisible.IsBound())
		Weapon->OnWeaponVisible.Broadcast();
}


void ACPlayer::Roll()
{
	//바라본 상태로 뒤로 뛰어야한다
	Movement->EnableControlRotation();

	if (InputComponent->GetAxisValue("MoveForward") < 0.0f)
		 Montages->PlayBackStepMode();

	else if (InputComponent->GetAxisValue("MoveRight") > 0.0f)
		Montages->PlayRightRollMode();

	else if (InputComponent->GetAxisValue("MoveRight") < 0.0f)
		Montages->PlayLeftRollMode();
	
}

void ACPlayer::Avoid()
{
	Movement->EnableControlRotation();
	//Movement->Move();

	Montages->PlayAvoidMode();
}

void ACPlayer::AirMode()
{
	SubState->SetAirComboMode();
}

void ACPlayer::AirComboLaunch()
{
	CheckFalse(bLaunch);

	AirCombo->Launch(this);

	GetCharacterMovement()->GravityScale = 0;
}

void ACPlayer::Execute()
{
	bExecute = true;
}

void ACPlayer::Turn()
{
	if (Movement->TurnLeft() == true)
		Montages->PlayTurnLeftMode();

	if (Movement->TurnRight() == true)
		Montages->PlayTurnRightMode ();
}

void ACPlayer::End_Roll()
{
	IICharacter::End_Roll();

	Movement->EnableControlRotation();
	State->SetIdleMode();
}

void ACPlayer::End_AirMode()
{
	SubState->SetGroundMode();

	bLaunch = false;
	bCheck = false;
	AirCombo->Fall(this);
	GetCharacterMovement()->GravityScale = 1;

}

void ACPlayer::Hitted()
{
	Status->Damage(Damage.Power);	//HP 대미지 처리
	  Damage.Power = 0;

	 
	if (!!Damage.Event && !!Damage.Event->HitData)
	{
		FHitData* data = Damage.Event->HitData;

		data->PlayMontage(this);
		data->PlayHitStop(GetWorld());
		data->PlaySoundWave(this);
		data->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());


		if (Status->IsDead() == false)	// 죽으면서 아래로 빠지는 것 방지함
		{

			//launch character
			FVector  start = GetActorLocation();
			FVector target = Damage.Character->GetActorLocation();
			FVector direction = target - start;
			direction.Normalize();
  
			LaunchCharacter(-direction * data->Launch, false, false);

			FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(start, target);

			SetActorRotation(FRotator(0, lookAtRotation.Yaw, 0));

		}
	}

	if (Status->IsDead())
	{
		//사망 처리
		State->SetDeadMode();

		return;
	}

	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;

}

void ACPlayer::End_Hitted()
{
	if (State->IsParryingMode())
	{
		CHelpers::DelayWorldTime(this, 1, true);
		Zoom->SetZoomValue(10);

	}
	if (State->IsFrozeMode())	//작은용한테 잡혔을때 Froze모드, Hitted에서 idle로 돌아와버리는것을 방지하기 위함
		State->SetFrozeMode();
	else
		State->SetIdleMode();


	bAvoidAttack = false;

	Movement->Move();
	Movement->DisableFixedCamera();

	bExecute = false;
}

float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	CheckTrueResult(State->IsHittedMode(), 0);

	//player를 공격한 enemy 가져오기
	enemy = Cast<ACEnemy_AI>(EventInstigator->GetPawn());
	CheckNullResult(enemy, 0);
	UCStateComponent* enemyState = CHelpers::GetComponent<UCStateComponent>(enemy);
	
	FVector player = GetActorLocation();
	FVector target = enemy->GetActorLocation();
	launchDirection = target - player;

	ACEnemy_Human* human = Cast<ACEnemy_Human>(enemy);
	if(!!human)
	{
		if (human->CanParrying() && this->bCanParrying)	//패링 성공
		{
			this->State->SetParryingMode();
			enemyState->SetParryingMode();

			CHelpers::DelayWorldTime(this, 0.8, true);
			Zoom->SetZoomValue(-10);
		
			return 0;
		}
		else if (human->CanParrying() && this->State->IsAvoidMode() && bAvoidAttack == false)	//회피기 저스트러시 회피후 공격
		{	//회피기
			bAvoidAttack = true;

			enemyState->SetParryingMode();

			return 0;
		}

	}
	
	//Defendmode || Parryingmode에서 대미지 0, 리턴
	CheckTrueResult(State->IsDefendMode(), 0);


	//대미지
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	Damage.Power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;

	State->SetHittedMode();

	return damage;
}

void ACPlayer::Landed(const FHitResult& HitResult)
{
	bCanJump = true;
	JumpCount = 0;

	bFall = false;
	if (SubState->IsGroundMode() == false)
	{
		PlayAnimMontage(LandedMontage, 1);
		SubState->SetGroundMode();
	}

	AirCombo->InAir(false);


	Parkour->DoParkour(true);
}

void ACPlayer::SetZooming(float InValue)
{
	CheckTrue(Weapon->IsBowMode());

	Zoom->SetZoomValue(InValue);
}

void ACPlayer::ShowMenu()
{
	CheckFalse(State->IsIdleMode());

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	weaponMenu->SetVisibility(ESlateVisibility::Visible);

	controller->SetShowMouseCursor(true);
	UWidgetBlueprintLibrary::SetInputMode_GameAndUI(controller);

}

void ACPlayer::HideMenu()
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	weaponMenu->SetVisibility(ESlateVisibility::Hidden);

	controller->SetShowMouseCursor(false);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(controller);

}

void ACPlayer::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	IGenericTeamAgentInterface::SetGenericTeamId(NewTeamID);

	if (TeamID != NewTeamID)
		TeamID = NewTeamID;

}
