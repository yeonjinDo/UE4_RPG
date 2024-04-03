#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponStructures.h"
#include "CRandomObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBossObjectHit, class AActor*, InCauser, class ACharacter*, InOtherCharacter);

UCLASS()
class CPORTFOLIO_API ACRandomObject : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere)
		class UNiagaraComponent* Niagara;



private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FHitData HitData;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float DamageInterval = 1;


private:
	UPROPERTY(EditAnywhere)
		FVector Size = FVector(2, 2, 2);

	//UPROPERTY(EditAnywhere)
	//	float LifeSpan = 2;

private:
	UPROPERTY(EditAnywhere)
		float Speed = 1;

	UPROPERTY(EditAnywhere)
		float DistanceFromStart = 200;

	UPROPERTY(EditAnywhere)
		float DistanceFromEnd = 100;


public:	
	ACRandomObject();

protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaTime) override;


private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:
	void Init(FVector Start, FVector End);
	void SetAttacker(ACharacter* attacker);

private:
	float Trail(float a, float b, float c, float d);


private:
	TArray<class ACharacter*> Hitted;
	FTimerHandle TimerHandle;

	
private:
	FVector PosA;
	FVector PosB;
	FVector PosC;
	FVector PosD;

	float CurrentTime = 0;
	float Angle;

	float MaxTime;

public:
	FBossObjectHit OnHit;
	ACharacter* Attacker;
	bool StopTick = false;
};
