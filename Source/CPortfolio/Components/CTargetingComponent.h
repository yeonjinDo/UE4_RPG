#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CTargetingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPORTFOLIO_API UCTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		float TraceDistance = 1000;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<EDrawDebugTrace::Type> DebugType;

	UPROPERTY(EditAnywhere)
		UParticleSystem* ParticleAsset;

	UPROPERTY(EditAnywhere)
		float FinishAngle = 5;

	UPROPERTY(EditAnywhere)
		float InterpSpeed = 15;

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* Particle;



public:	
	UCTargetingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void BeginTargeting();
	void EndTargeting();
	void ChangeTarget(ACharacter* InCandidate);
	void TickTargeting();
	void ChangeFocus(bool bInRight);

public:
	void ToggleTarget();
	void ChangeTargetLeft();
	void ChangeTargetRight();


private:
	ACharacter* OwnerCharacter;
	ACharacter* TargetCharacter;

	bool MovingFocus = false;


	
};
