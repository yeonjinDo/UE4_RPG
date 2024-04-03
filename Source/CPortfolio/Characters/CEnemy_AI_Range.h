#pragma once

#include "CoreMinimal.h"
#include "CEnemy_Human.h"
#include "Characters/CEnemy_AI.h"
#include "CEnemy_AI_Range.generated.h"

UCLASS()
class CPORTFOLIO_API ACEnemy_AI_Range : public ACEnemy_Human
{
	GENERATED_BODY()

public:
	ACEnemy_AI_Range();

public:
	virtual void BeginPlay() override;
	virtual FGenericTeamId GetGenericTeamId() const override;
};
