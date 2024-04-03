#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CNotifyBox.generated.h"

UCLASS()
class CPORTFOLIO_API ACNotifyBox : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Cube;

private:
	UPROPERTY(EditAnywhere)
		FVector BoxSize = FVector(10, 1, 1);

public:	
	ACNotifyBox();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
