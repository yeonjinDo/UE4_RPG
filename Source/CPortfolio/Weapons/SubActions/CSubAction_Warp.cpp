#include "Weapons/SubActions/CSubAction_Warp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

void UCSubAction_Warp::OnCoolDown()
{
	FTimerDelegate delegate = FTimerDelegate::CreateLambda([&]()
	{
		if (CurrentTime <= CoolTime)
			CurrentTime += 0.1f;
	});

	Owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle, delegate, 0.1, true, 0);
	
}

UCSubAction_Warp::UCSubAction_Warp()
{
	CameraActorClass = ACameraActor::StaticClass();
	CoolTime = 5.0f;

	OnSubActionCoolDown.AddDynamic(this, &UCSubAction_Warp::OnCoolDown);
}

void UCSubAction_Warp::Pressed()
{
	CheckNull(PlayerController);
	CheckTrue(State->IsSubActionMode());

	Super::Pressed();

	//CoolTime
	if (CurrentTime <= CoolTime)
	{
		bInAction = false;
		return;
	}

	CurrentTime = 0;
	//CoolTime


	State->OnSubActionMode();
	Movement->EnableTopViewCamera();
	PlayerController->SetViewTargetWithBlend(CameraActor, BlendIn);

	SkillNum = 4;
}

void UCSubAction_Warp::Released()
{
	CheckNull(PlayerController);
	CheckFalse(State->IsSubActionMode());

	Super::Released();
	if (OnSubActionCoolDown.IsBound())
		OnSubActionCoolDown.Broadcast();

	State->OffSubActionMode();
	Movement->DisableTopViewCamera();
	PlayerController->SetViewTargetWithBlend(Owner, BlendOut);
	
}

void UCSubAction_Warp::BeginPlay(ACharacter * InOwner, ACAttachment * InAttachment, UCDoAction * InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	PlayerController = InOwner->GetController<APlayerController>();

	//카메라액터 회전시켜주기(플레이어 머리 위로)
	CameraActor = InOwner->GetWorld()->SpawnActor<ACameraActor>(CameraActorClass);
	CameraActor->SetActorRotation(FRotator(-90, 0, 0));

	//기본 설정
	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(CameraActor);
	camera->ProjectionMode = ProjectionMode;
	camera->OrthoWidth = OrthoWidth;
	camera->FieldOfView = FieldOfView;

}

void UCSubAction_Warp::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	CheckNull(PlayerController);
	CheckNull(CameraActor);

	//플레이어 이동에 맞게 카메라 움직이는거
	CameraActor->SetActorLocation(Owner->GetActorLocation() + FVector(0, 0, CameraRelativeHeight));
}
