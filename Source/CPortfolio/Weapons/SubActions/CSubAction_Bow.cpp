#include "Weapons/SubActions/CSubAction_Bow.h"

#include "AIController.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/CPlayer.h"
#include "Components/CStateComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Widgets/CUserWidget_CrossHair.h"


UCSubAction_Bow::UCSubAction_Bow()
{
	CHelpers::GetAsset<UCurveVector>(&Curve, "CurveVector'/Game/Weapons/Bow/Curve_Aim.Curve_Aim'");

	CHelpers::GetClass<UCUserWidget_CrossHair>(&CrossHairClass, "WidgetBlueprint'/Game/Widgets/WB_CrossHair.WB_CrossHair_C'");

}

void UCSubAction_Bow::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	Camera    = CHelpers::GetComponent<UCameraComponent>(InOwner);


	//만들어놓은 Curve_Aim 실행
	FOnTimelineVector timeline;
	timeline.BindUFunction(this, "OnAiming");

	Timeline.AddInterpVector(Curve, timeline);
	//커브를 0초~20초 구간으로 만들어놨다 -> 200배 빠르게 재생하면 0.1초 만에 재생되게 된다
	Timeline.SetPlayRate(AimingSpeed);

	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);

	if (!!bow)
		Bend = bow->GetBend();

	if(!!CrossHairClass)
	{
		ACPlayer* player = Cast<ACPlayer>(Owner);
		CheckNull(player);

		CrossHair = CreateWidget<UCUserWidget_CrossHair, APlayerController>(Owner->GetController<APlayerController>(), CrossHairClass);
		CrossHair->AddToViewport();
		CrossHair->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UCSubAction_Bow::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	Timeline.TickTimeline(InDeltaTime);

	CheckNull(CrossHair);

	//CrossHair linetrace
	FVector start = Owner->GetMesh()->GetSocketLocation("Hand_Bow_Right_Arrow");
	AActor* owner = Cast<AActor>(Owner->GetController());
	
	FVector end = start + owner->GetActorForwardVector() * 2000;
	

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TEnumAsByte<EObjectTypeQuery> pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	objectTypes.Add(pawn);

	TArray<AActor*> ignores;
	ignores.Add(Owner);

	FHitResult hitresult;

	UKismetSystemLibrary::LineTraceSingleForObjects(Owner->GetWorld(), start, end, objectTypes, false, 
		ignores, EDrawDebugTrace::None, hitresult, true);


	if(hitresult.bBlockingHit)
	{
		ACharacter* hitCharacter = Cast<ACharacter>(hitresult.Actor);

		if(!!hitCharacter)
			CrossHair->CrossHair_Red();
		else
			CrossHair->CrossHair_White();
	}
	else
		CrossHair->CrossHair_White();
	//
}

void UCSubAction_Bow::OnAiming(FVector Output)
{
	Camera->FieldOfView = Output.X;

	if (!!Bend)
		*Bend = Output.Y;

}

void UCSubAction_Bow::Pressed()
{
	CheckTrue(State->IsSubActionMode());

	//AI_Range가 Aim상태를 기본 유지 할 수 있도록
	if(!!Owner->GetController<AAIController>())
	{
		Super::Pressed();
		State->OnSubActionMode();

		return;
	}

	CheckNull(SpringArm);
	CheckNull(Camera);


	Super::Pressed();

	State->OnSubActionMode();

	if (!!CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Visible);

	OriginData.TargetArmLength  = SpringArm->TargetArmLength;
	OriginData.SocketOffset     = SpringArm->SocketOffset;
	OriginData.bEnableCameraLeg = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation   = Camera->GetRelativeLocation();

	
	SpringArm->TargetArmLength    = AimData.TargetArmLength;
	SpringArm->SocketOffset       = AimData.SocketOffset;
	SpringArm->bEnableCameraLag   = AimData.bEnableCameraLeg;
	Camera->SetRelativeLocation(AimData.CameraLocation);

	Timeline.PlayFromStart();

}

void UCSubAction_Bow::Released()
{
	CheckFalse(State->IsSubActionMode());

	if (!!Owner->GetController<AAIController>())
	{
		Super::Released();
		State->OffSubActionMode();

		return;
	}


	CheckNull(SpringArm);
	CheckNull(Camera);

	Super::Released();

	State->OffSubActionMode();

	if (!!CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Hidden);

	SpringArm->TargetArmLength    = OriginData.TargetArmLength;
	SpringArm->SocketOffset       = OriginData.SocketOffset;
	SpringArm->bEnableCameraLag   = OriginData.bEnableCameraLeg;
	Camera->SetRelativeLocation(OriginData.CameraLocation);

	Timeline.ReverseFromEnd();

}