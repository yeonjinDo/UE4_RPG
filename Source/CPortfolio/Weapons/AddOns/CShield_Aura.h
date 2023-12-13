#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CShield_Aura.generated.h"

UCLASS()
class CPORTFOLIO_API ACShield_Aura : public AActor
{
	GENERATED_BODY()


private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle;


public:
	ACShield_Aura();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);	//끝날때 타이머 제거시 사용


public:
	virtual void Tick(float DeltaTime) override;
};
