#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CNotify_Around.generated.h"

UCLASS()
class CPORTFOLIO_API ACNotify_Around : public AActor
{
	GENERATED_BODY()


private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere)
		class UParticleSystemComponent* Particle;



private:
	UPROPERTY(EditAnywhere)
		int32 Speed = 2;

	UPROPERTY(EditAnywhere)
		int32 Max = 1000;


public:	
	ACNotify_Around();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	bool GetGroundLocation(FVector& OutLocation);

public:
	FVector GetNotifyLocation();
	FRotator GetNotifyRotation();

private:
	int CircleLocation = 0;
	bool Negative = false;

};
