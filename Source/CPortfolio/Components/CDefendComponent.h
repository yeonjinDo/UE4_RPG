#pragma once

#include "CoreMinimal.h"
#include "Characters/ICharacter.h"
#include "Components/ActorComponent.h"
#include "CDefendComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPORTFOLIO_API UCDefendComponent : public UActorComponent, public IICharacter
{
	GENERATED_BODY()

public:	
	UCDefendComponent();

public:
	FORCEINLINE void CanParrying(bool b) { bCanParrying = b; }
	FORCEINLINE bool CanParrying() { return bCanParrying; }


protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void OnDefend();
	void Defend();
	void Parrying();
	void EndDefend();

	void End_Defend() override;


private:
	class ACharacter* OwnerCharacter;
	class UCMontagesComponent* Montages;
	class UCStateComponent* State;

private:
	bool bCanParrying;

};
