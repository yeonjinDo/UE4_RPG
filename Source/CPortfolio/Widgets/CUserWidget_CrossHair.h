#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_CrossHair.generated.h"

UCLASS()
class CPORTFOLIO_API UCUserWidget_CrossHair : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeOnInitialized() override;


private:
	class UImage* img;

public:
	void CrossHair_White();
	void CrossHair_Red();

};
