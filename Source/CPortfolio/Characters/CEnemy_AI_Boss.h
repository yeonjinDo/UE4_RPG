#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy_AI.h"
#include "CEnemy_AI_Boss.generated.h"

UCLASS()
class CPORTFOLIO_API ACEnemy_AI_Boss : public ACEnemy_AI
{
	GENERATED_BODY()


#if WITH_EDITOR	
private:
	UPROPERTY(VisibleDefaultsOnly)
		TSubclassOf<class UCUserWidget_BossStatus> HpBar;

#endif


public:
	ACEnemy_AI_Boss();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


private:
	void Hitted() override;

private:
	void End_AIAction() override;
	void End_Dead() override;

private:
	FTimerHandle RestoreColor_TimerHandle;	


#if WITH_EDITOR
private:
	UUserWidget* widget;
#endif
};
