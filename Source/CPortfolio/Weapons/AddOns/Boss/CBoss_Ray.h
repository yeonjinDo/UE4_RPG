#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponStructures.h"
#include "NiagaraDataInterfaceExport.h"
#include "CBoss_Ray.generated.h"

UCLASS()
class CPORTFOLIO_API ACBoss_Ray : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FHitData HitData;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float DamageInterval = 0.7;


private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	
	UPROPERTY(EditDefaultsOnly)
		class UParticleSystemComponent* Particle;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* Box;



public:	
	ACBoss_Ray();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


private:

	UFUNCTION()
		void OnSystemFinished(class UParticleSystemComponent* PSystem);

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	TArray<class ACharacter*> Hitted;
	FTimerHandle TimerHandle;



};
