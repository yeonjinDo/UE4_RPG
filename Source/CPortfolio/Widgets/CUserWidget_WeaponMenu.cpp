#include "Widgets/CUserWidget_WeaponMenu.h"

#include "CUserWidget_WeaponButton.h"
#include "Global.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/CPlayer.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Kismet/KismetStringLibrary.h"

void UCUserWidget_WeaponMenu::NativeConstruct()
{
	Super::NativeConstruct();

	UTexture2D* texture;
	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/Fist.Fist'");
	Textures.Add(texture);

	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/Sword.Sword'");
	Textures.Add(texture);

	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/Hammer.Hammer'");
	Textures.Add(texture);

	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/Warp.Warp'");
	Textures.Add(texture);

	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/Around.Around'");
	Textures.Add(texture);

	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/Bow.Bow'");
	Textures.Add(texture);



	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	for (UWidget* widget: widgets)
	{
		Grid = Cast<UGridPanel>(widget);

		if(!!Grid)
		{
			int index = 0;
			
			for (UWidget* gridWidget : Grid->GetAllChildren())
			{
			
				Button = Cast<UCUserWidget_WeaponButton>(gridWidget);

				UWidgetBlueprintLibrary::SetBrushResourceToTexture(Button->ImageButton->WidgetStyle.Normal,  Textures[index]);
				UWidgetBlueprintLibrary::SetBrushResourceToTexture(Button->ImageButton->WidgetStyle.Hovered, Textures[index]);
				UWidgetBlueprintLibrary::SetBrushResourceToTexture(Button->ImageButton->WidgetStyle.Pressed, Textures[index]);

				index++;
			}
		}
	}

		
	
	 
}

void UCUserWidget_WeaponMenu::OnClicked(FString InName)
{
	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	for (UWidget* widget : widgets)
	{
		Grid = Cast<UGridPanel>(widget);

		if (!!Grid)
		{
			for (UWidget* gridWidget : Grid->GetAllChildren())
			{
				Button = Cast<UCUserWidget_WeaponButton>(gridWidget);

				if (UKismetSystemLibrary::GetObjectName(Button) == InName)
				{
					ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
					UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(player);

					player->HideMenu();

					if (!!weapon)
					{
						FString name = UKismetStringLibrary::Replace(InName, "Action_", "");
						int index = UKismetStringLibrary::Conv_StringToInt(name);

						switch (index)
						{
						case 0:
							weapon->SetFistMode();
							break;
						case 1:
							weapon->SetSwordMode();
							break;
						case 2:
							weapon->SetHammerMode();
							break;
						case 3:
							weapon->SetWarpMode();
							break;
						case 4:
							weapon->SetAroundMode();
							break;
						case 5:
							weapon->SetBowMode();
							break;

						}
					}
				}
			}
		}
	}
}

void UCUserWidget_WeaponMenu::OnHovered(FString InName)
{
	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	for (UWidget* widget : widgets)
	{
		Grid = Cast<UGridPanel>(widget);

		if (!!Grid)
		{
			for (UWidget* gridWidget : Grid->GetAllChildren())
			{
				if (UKismetSystemLibrary::GetObjectName(gridWidget) == InName)
				{
					Button = Cast<UCUserWidget_WeaponButton>(gridWidget);
					Button->Select->SetBrushColor(FLinearColor::Red);
				}
			}
		}
	}


}

void UCUserWidget_WeaponMenu::OnUnhovered(FString InName)
{
	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	for (UWidget* widget : widgets)
	{
		Grid = Cast<UGridPanel>(widget);

		if (!!Grid)
		{
			for (UWidget* gridWidget : Grid->GetAllChildren())
			{
				if (UKismetSystemLibrary::GetObjectName(gridWidget) == InName)
				{
					Button = Cast<UCUserWidget_WeaponButton>(gridWidget);

					Button->Select->SetBrushColor(FLinearColor::White);
				}
			}
		}
	}

}

 

void UCUserWidget_WeaponMenu::OnClicked_Break()
{
}

void UCUserWidget_WeaponMenu::OnHovered_Break()
{
}

void UCUserWidget_WeaponMenu::OnUnhovered_Break()
{
}
