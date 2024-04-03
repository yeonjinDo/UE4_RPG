#pragma once

#include "CoreMinimal.h"
#include "CEnemy_Human.h"
#include "CEnemy_AI_Skeleton.generated.h"

UCLASS()
class CPORTFOLIO_API ACEnemy_AI_Skeleton : public ACEnemy_Human
{
	GENERATED_BODY()

public:
	ACEnemy_AI_Skeleton();

public:
	virtual void BeginPlay() override;

	virtual FGenericTeamId GetGenericTeamId() const override;

};
