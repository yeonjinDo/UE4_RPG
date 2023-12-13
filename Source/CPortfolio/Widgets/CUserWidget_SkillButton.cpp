#include "Widgets/CUserWidget_SkillButton.h"
#include "Global.h"
#include "Blueprint/WidgetTree.h"
#include "Components/ProgressBar.h"


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

	if(CurrentTime == 0)
	{
		bStartTick = true;
	}


	if(bStartTick)
	{
		CurrentTime += InDeltaTime;

		if (CurrentTime >= 5.0f)
			bStartTick = false;
	}
		

	CoolTime->SetPercent(CurrentTime / 5.0f);
}
