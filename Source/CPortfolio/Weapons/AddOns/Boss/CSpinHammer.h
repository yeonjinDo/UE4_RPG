#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponStructures.h"
#include "CSpinHammer.generated.h"

UCLASS()
class CPORTFOLIO_API ACSpinHammer : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* Skeletal;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* RotationPoint;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

	UPROPERTY(EditAnywhere, Category = "Spinning")
		class UCurveFloat* Curve;

	UPROPERTY(EditAnywhere, Category = "Spinning")
		float SpinSpeed = 100;

	UPROPERTY(EditAnywhere, Category = "Particle")
		class UParticleSystemComponent* Particle;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FHitData HitData;

	UPROPERTY(EditAnywhere, Category = "CameraShake")
		TSubclassOf<class UMatineeCameraShake> CameraShakeClass;



private:
	UFUNCTION()
		void OnSpinning(float Output);

private:
	UFUNCTION()
		void OnTimeLineFinished();

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:	
	ACSpinHammer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


public:
	void Spin();

private:
	FTimeline Timeline;
	FQuat OriginRotation;

};

