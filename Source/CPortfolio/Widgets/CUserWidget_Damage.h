#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Damage.generated.h"

UCLASS()
class CPORTFOLIO_API UCUserWidget_Damage : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void SpawnDamage(float InDamage, FVector InLocation);

};
