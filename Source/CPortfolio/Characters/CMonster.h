#pragma once

#include "CoreMinimal.h"
#include "CEnemy_AI.h"
#include "GenericTeamAgentInterface.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "CMonster.generated.h"

UCLASS()
class CPORTFOLIO_API ACMonster : public ACEnemy_AI
{
	GENERATED_BODY()


public:
	ACMonster();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);


public:
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void Hitted();
	void Dead();

public:
	virtual void End_Hitted() override;
	virtual void End_Dead() override;


	virtual FGenericTeamId GetGenericTeamId() const override;


};
