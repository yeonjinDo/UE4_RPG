#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CNotifyCircle.generated.h"

UCLASS()
class CPORTFOLIO_API ACNotifyCircle : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere)
		class UParticleSystemComponent* Particle;

private:
	UPROPERTY(EditAnywhere)
		FVector CircleSize = FVector(2,2,2);

public:	
	ACNotifyCircle();
	
protected:
	virtual void BeginPlay() override;


};
