#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Label.generated.h"

UCLASS()
class CPORTFOLIO_API UCUserWidget_Label : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Label")
		void UpdateHealth(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "Label")
		void UpdateName(const FText& InName);

	
};
