#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy_AI.h"
#include "CEnemy_AI_Range.generated.h"

UCLASS()
class CPORTFOLIO_API ACEnemy_AI_Range : public ACEnemy_AI
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual FGenericTeamId GetGenericTeamId() const override;
};
