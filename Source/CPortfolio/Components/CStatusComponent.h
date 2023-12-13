#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPORTFOLIO_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		TSubclassOf<class UCUserWidget_Message> Message;

private:
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 1000;

	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxMana = 1000;

public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetMaxMana() { return MaxMana; }

public:
	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE bool IsDead() { return Health <= 0.0f; }

	FORCEINLINE float GetMana() { return Mana; }
	


public:	
	UCStatusComponent();


protected:
	virtual void BeginPlay() override;


public:
	void Damage(float InAmount);
	void UseMana(float InAmount);
	void RecoverMana(float InMana);
	bool EmptyMana();

private:
	class ACharacter* OwnerCharacter;

private:
	float Health;
	float Mana;
	UUserWidget*  widget;

	FTimerHandle TimerHandle;
	float TimerInterval = 2.0f;

};
