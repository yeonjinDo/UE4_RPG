#include "Widgets/CUserWidget_SkillButton.h"
#include "Global.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/CPlayer.h"
#include "Components/ProgressBar.h"
#include "Weapons/CSkillAction.h"
#include "Weapons/CSubAction.h"


void UCUserWidget_SkillButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	for (UWidget* widget : widgets)
	{
		if (!!Cast<UProgressBar>(widget))
			CoolTime = Cast<UProgressBar>(widget);

	}


}

void UCUserWidget_SkillButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(bStartTick)
	{
		CurrentTime += InDeltaTime;
		
		if (CurrentTime >= MaxCoolTime)
			bStartTick = false;

		CoolTime->SetPercent(CurrentTime / MaxCoolTime);
	}
	

}
