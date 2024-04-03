#include "Widgets/CUserWidget_WeaponButton.h"
#include "CUserWidget_WeaponMenu.h"
#include "Blueprint/WidgetTree.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"


void UCUserWidget_WeaponButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> widgets;

	WidgetTree->GetAllWidgets(widgets);

	for (UWidget* widget : widgets)
	{
		ImageButton = Cast<UButton>(widget);

		Border = Cast<UBorder>(widget);
		if (!!Border)
			Select = Border;
	}
	
	

	ImageButton->OnClicked.AddDynamic(this, &UCUserWidget_WeaponButton::OnClicked);
	ImageButton->OnHovered.AddDynamic(this, &UCUserWidget_WeaponButton::OnHovered);
	ImageButton->OnUnhovered.AddDynamic(this, &UCUserWidget_WeaponButton::OnUnhovered);

}

void UCUserWidget_WeaponButton::OnClicked()
{
	ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	CheckNull(player);

	player->weaponMenu->OnClicked(UKismetSystemLibrary::GetObjectName(this));

}

void UCUserWidget_WeaponButton::OnHovered()
{
	ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	CheckNull(player);

	player->weaponMenu->OnHovered(UKismetSystemLibrary::GetObjectName(this));

}

void UCUserWidget_WeaponButton::OnUnhovered()
{
	ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	CheckNull(player);

	player->weaponMenu->OnUnhovered(UKismetSystemLibrary::GetObjectName(this));

}

