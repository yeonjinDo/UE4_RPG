#pragma once

#include "CoreMinimal.h"
#include "CEnemy_Human.h"
#include "Characters/CEnemy_AI.h"
#include "CEnemy_A_Nerd.generated.h"

UCLASS()
class CPORTFOLIO_API ACEnemy_A_Nerd : public ACEnemy_Human
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual FGenericTeamId GetGenericTeamId() const override;

};
