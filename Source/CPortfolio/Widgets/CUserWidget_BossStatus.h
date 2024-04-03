#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_BossStatus.generated.h"

UCLASS()
class CPORTFOLIO_API UCUserWidget_BossStatus : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHealth(float InHealth, float InMaxHealth);

};
