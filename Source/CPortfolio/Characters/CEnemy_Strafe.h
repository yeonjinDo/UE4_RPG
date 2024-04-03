#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy_Human.h"
#include "CEnemy_Strafe.generated.h"

UCLASS()
class CPORTFOLIO_API ACEnemy_Strafe : public ACEnemy_Human
{
	GENERATED_BODY()

public:
	ACEnemy_Strafe();

public:
	virtual void BeginPlay() override;

	virtual FGenericTeamId GetGenericTeamId() const override;
};
