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
	Textures.Add(texture);	//일섬

	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/SkillAction_ThrowShield.SkillAction_ThrowShield'");
	Textures.Add(texture);	//ThrowShield

	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/SubAction_Hammer.SubAction_Hammer'");
	Textures.Add(texture);	//subaction_hammer

	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/SubAction_Around.SubAction_Around'");
	Textures.Add(texture);	//subAction_around

	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/SubAction_Warp.SubAction_Warp'");
	Textures.Add(texture);	//subaction_warp - change view

	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/Dash.Dash'");
	Textures.Add(texture);	//Dash

	CHelpers::GetAsset<UTexture2D>(&texture, "Texture2D'/Game/Widgets/Widget_Asset/Icon/Extra.Extra'");
	Textures.Add(texture);	//기능없음

	

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
	{
		player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		
		//weapon->OnWeaponTypeChange.AddDynamic(this, &UCUserWidget_SkillMenu::SetSkil);
	}

	weapon = CHelpers::GetComponent<UCWeaponComponent>(player);
	UCSubStateComponent* subState = CHelpers::GetComponent<UCSubStateComponent>(player);

	if (subState->IsDashMode())
		SetSkill(8);

	CheckNull(weapon);

	CheckNull(weapon->GetSubAction());
	if (weapon->GetSubAction()->GetInAction())
		SetSkill(weapon->GetSubAction()->GetSkillNum());

	CheckNull(weapon->GetSkillAction());
	if (weapon->GetSkillAction()->GetInAction())
		SetSkill(weapon->GetSkillAction()->GetSkillNum());

	
}

void UCUserWidget_SkillMenu::SetSkill(int InSkillNum)
{
	CheckFalse(InSkillNum > 0);

	Buttons[InSkillNum - 1]->SetCurrentTime(0);
	//SkillNum = (int)InNewType;
}
