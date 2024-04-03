#include "Widgets/CUserWidget_CrossHair.h"
#include "Global.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"


void UCUserWidget_CrossHair::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UCanvasPanel* panel = Cast<UCanvasPanel>(WidgetTree->RootWidget);
	CheckNull(panel);


	TArray<UWidget*> widgets = panel->GetAllChildren();
	for (UWidget* widget : widgets)
	{
		img = Cast<UImage>(widget);
	}

}

void UCUserWidget_CrossHair::CrossHair_White()
{
	img->SetBrushTintColor(FLinearColor::White);
}

void UCUserWidget_CrossHair::CrossHair_Red()
{
	img->SetBrushTintColor(FLinearColor::Red);
}
