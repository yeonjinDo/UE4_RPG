#include "Components/CStatusComponent.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "Widgets/CUserWidget_Message.h"


UCStatusComponent::UCStatusComponent()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	Health = MaxHealth;
	Mana = MaxMana;

#if WITH_EDITOR

	CHelpers::GetClass<UCUserWidget_Message>(&Message, "WidgetBlueprint'/Game/Widgets/WB_Message.WB_Message_C'");
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	widget = CreateWidget(controller, Message);

	if (!!widget)
	{
		widget->AddToViewport();
		widget->SetVisibility(ESlateVisibility::Hidden);
	}

#endif

}


void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCStatusComponent::Damage(float InAmount)
{
	Health += (InAmount * -1.0f);
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::UseMana(float InAmount)
{
	Mana += (InAmount * -1.0f);
	Mana = FMath::Clamp(Mana, 0.0f, MaxMana);
}

void UCStatusComponent::RecoverMana(float InAmount)
{
	Mana += (InAmount * 1.0f);
	Mana = FMath::Clamp(Mana, 0.0f, MaxMana);
}

bool UCStatusComponent::EmptyMana()
{
	if(!!widget)
	{
		if (Mana <= 2.0f)
		{
			if (Mana >= -2.0f)
				Mana = -10.0f;

			widget->SetVisibility(ESlateVisibility::Visible);
			return true;
		}

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this] {

			widget->SetVisibility(ESlateVisibility::Hidden);
			return false;

		}, TimerInterval, false);

		return false;
	}

	return false;
}
