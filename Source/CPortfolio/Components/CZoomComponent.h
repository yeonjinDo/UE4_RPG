#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CZoomComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPORTFOLIO_API UCZoomComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Zooming")
		float Speed = 20;

	UPROPERTY(EditAnywhere, Category = "Zooming")
		float InterpSpeed = 5;

	UPROPERTY(EditAnywhere, Category = "Zooming")
		FVector2D Range = FVector2D(100, 500);


public:	
	UCZoomComponent();


protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetZoomValue(float InValue);

private:
	class USpringArmComponent* SpringArm;

	float CurrentValue;
};
