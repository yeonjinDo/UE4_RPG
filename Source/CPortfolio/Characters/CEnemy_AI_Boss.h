#pragma once

#include "CoreMinimal.h"
#include "CEnemy_Human.h"
#include "CEnemy_AI_Boss.generated.h"

UCLASS()
class CPORTFOLIO_API ACEnemy_AI_Boss : public ACEnemy_Human
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


protected:
	virtual void Hitted() override;
	virtual void End_Dead() override;

public:
	void End_Action();
	void End_Dash();

	bool GetEndAction() { return EndAction; }
	void SetEndAction(bool b) { EndAction = b; }

private:
	FTimerHandle RestoreColor_TimerHandle;	

	bool EndAction = false;

#if WITH_EDITOR
private:
	UUserWidget* widget;
#endif
};
