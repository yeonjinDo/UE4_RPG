#include "Characters/CEnemy_AI.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CStatusComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Widgets/CUserWidget_Damage.h"
#include "Widgets/CUserWidget_Label.h"


ACEnemy_AI::ACEnemy_AI()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCAIBehaviorComponent>(this, &Behavior, "Behavior");


#if WITH_EDITOR

	CHelpers::CreateComponent<UWidgetComponent>(this, &LabelWidget, "Label", GetMesh());

	TSubclassOf<UCUserWidget_Label> labelClass;
	CHelpers::GetClass<UCUserWidget_Label>(&labelClass, "WidgetBlueprint'/Game/Widgets/WB_Label.WB_Label_C'");
	LabelWidget->SetWidgetClass(labelClass);
	LabelWidget->SetRelativeLocation(FVector(0, 0, 220));
	LabelWidget->SetDrawSize(FVector2D(120, 0));
	LabelWidget->SetWidgetSpace(EWidgetSpace::Screen);

#endif

}


void ACEnemy_AI::BeginPlay()
{
   Super::BeginPlay();


#if WITH_EDITOR
   LabelWidget->InitWidget();

   label = Cast<UCUserWidget_Label>(LabelWidget->GetUserWidgetObject());
   label->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());

#endif

}

void ACEnemy_AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Status->GetHealth() <= 0)
		Behavior->SetDeadMode();


#if WITH_EDITOR
	label = Cast<UCUserWidget_Label>(LabelWidget->GetUserWidgetObject());

	if (!!label)
	{
		label->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());

		UpdateLabelRenderScale();
	}
#endif
}

void ACEnemy_AI::Hitted()
{
	Super::Hitted();
	
	Behavior->SetHittedMode();
}

void ACEnemy_AI::End_Hitted()
{
	Super::End_Hitted();

	Behavior->SetWaitMode();
}

void ACEnemy_AI::End_Dead()
{
	Super::End_Dead();

	if (OnDead.IsBound())
		OnDead.Broadcast();
}

#if WITH_EDITOR
void ACEnemy_AI::UpdateLabelRenderScale()
{
	label = Cast<UCUserWidget_Label>(LabelWidget->GetUserWidgetObject());
	CheckNull(label);

	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	FVector cameraLocation = cameraManager->GetCameraLocation();
	CheckNull(GetController());
	FVector targetLocation = GetController()->GetTargetLocation();

	float distance = FVector::Distance(cameraLocation, targetLocation);
	float sizeRate = 1.0f - (distance / LabelViewAmount);


	if (distance > LabelViewAmount)
	{
		label->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}


	label->SetVisibility(ESlateVisibility::Visible);
	label->SetRenderScale(FVector2D(sizeRate, sizeRate));
}
#endif
