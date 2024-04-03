#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_PlayerStatus.generated.h"

UCLASS()
class CPORTFOLIO_API UCUserWidget_PlayerStatus : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Label")
		void UpdateHealth(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "Label")
		void UpdateMana(float InMana, float InMaxMana);
};
