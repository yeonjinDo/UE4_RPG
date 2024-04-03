#pragma once

#include "CoreMinimal.h"
#include "Global.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"

UINTERFACE(MinimalAPI)
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

class CPORTFOLIO_API IICharacter
{
	GENERATED_BODY()

public:
	virtual void End_Roll() { }
	virtual void End_Defend() { }
	virtual void End_Hitted() { }
	virtual void End_Dead() { }

public:
	void Create_DynamicMaterial(class ACharacter* InCharacter);
	void Change_Color(class ACharacter* InCharacter, FLinearColor InColor);
	void LimitHeight(class ACharacter* InCharacter, float InLocation = 0);
	
};
