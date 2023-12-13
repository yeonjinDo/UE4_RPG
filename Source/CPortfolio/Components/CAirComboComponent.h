#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAirComboComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPORTFOLIO_API UCAirComboComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Launch")
		FVector LaunchVelocity = FVector(0, 0, 1500);

	UPROPERTY(EditAnywhere, Category = "Launch")
		FVector FallVelocity = FVector(0, 0, -1000);

public:
	FORCEINLINE bool& InAir() { return bInAir; }
	FORCEINLINE void InAir(bool b) { bInAir = b; }

public:	
	UCAirComboComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	void Launch(ACharacter* InCharacter);
	void Fall(ACharacter* InCharacter);

private:
	class ACharacter* OwnerCharacter;

private:
	bool bInAir = false;

};

