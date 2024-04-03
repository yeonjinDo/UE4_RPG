#include "Widgets/CUserWidget_SkillMenu.h"
#include "CUserWidget_SkillButton.h"
#include "Global.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/CPlayer.h"
#include "Components/GridPanel.h"
#include "Components/ProgressBar.h"
#include "Weapons/CSkillAction.h"
#include "Weapons/CSubAction.h"


void UCUserWidget_SkillMenu::NativeConstruct()
{
	Super::NativeConstruct();

	UTexture2D* texture;
	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/SubAction_Fist.SubAction_Fist'");
	Textures.Add(texture);	//SubAction_fist

	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/SubAction_Sword.SubAction_Sword'");
	Textures.Add(texture);	//¿œº∂

	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/SubAction_Hammer.SubAction_Hammer'");
	Textures.Add(texture);	//subaction_hammer

	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/SubAction_Warp.SubAction_Warp'");
	Textures.Add(texture);	//subaction_warp - change view

	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/SubAction_Around.SubAction_Around'");
	Textures.Add(texture);	//subAction_around

	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/SkillAction_ThrowShield.SkillAction_ThrowShield'");
	Textures.Add(texture);	//ThrowShield


	
	

	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	for (UWidget* widget : widgets)
	{
		Grid = Cast<UGridPanel>(widget);

		if (!!Grid)
		{
			int index = 0;

			for (UWidget* gridWidget : Grid->GetAllChildren())
			{

				Buttons.Add(Cast<UCUserWidget_SkillButton>(gridWidget));
				
				UWidgetBlueprintLibrary::SetBrushResourceToTexture(Buttons[index]->CoolTime->WidgetStyle.BackgroundImage, Textures[index]);
				UWidgetBlueprintLibrary::SetBrushResourceToTexture(Buttons[index]->CoolTime->WidgetStyle.FillImage, Textures[index]);
				
				index++;
			}
		}
	}

}

void UCUserWidget_SkillMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (player == nullptr)
		player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	weapon = CHelpers::GetComponent<UCWeaponComponent>(player);
	UCSubStateComponent* subState = CHelpers::GetComponent<UCSubStateComponent>(player);
	  
	CheckNull(weapon);

	CheckNull(weapon->GetSubAction());
	if (weapon->GetSubAction()->GetInAction())
	{
		SetSkill(weapon->GetSubAction()->GetSkillNum(), weapon->GetSubAction()->GetCoolTime());
	}

	CheckNull(weapon->GetSkillAction());
	if (weapon->GetSkillAction()->GetInAction())
	{
		SetSkill(weapon->GetSkillAction()->GetSkillNum(), weapon->GetSkillAction()->GetCoolTime());
	}
	
}

void UCUserWidget_SkillMenu::SetSkill(int InSkillNum, float InCoolTime)
{
	CheckFalse(InSkillNum > 0);

	Buttons[InSkillNum - 1]->SetCurrentTime(0);
	Buttons[InSkillNum - 1]->SetStartTick(true);
	Buttons[InSkillNum - 1]->SetMaxCoolTime(InCoolTime);
	
	//SkillNum = (int)InNewType;
}
