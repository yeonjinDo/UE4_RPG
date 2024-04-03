#pragma once

#include "CoreMinimal.h"
#include "CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Characters/CEnemy.h"
#include "CEnemy_Human.generated.h"

UCLASS()
class CPORTFOLIO_API ACEnemy_Human : public ACEnemy_AI
{
	GENERATED_BODY()

public:
	ACEnemy_Human();

public:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


protected:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
		void OnSubStateTypeChanged(ESubStateType InPrevType, ESubStateType InNewType);



public:
	FORCEINLINE bool& CanParrying() { return bCanParrying; }
	FORCEINLINE void CanParrying(bool b) { bCanParrying = b; }

	

private:
	void Landed(const FHitResult& HitResult);
	void Rise();
	void Avoid();

protected:
	virtual void Parrying();
	virtual void AirComboLaunch();

private:
	bool bCanParrying = false;

protected:
	bool bCheck = false;	 // limit height ¿ë
	float height = 0;

};


