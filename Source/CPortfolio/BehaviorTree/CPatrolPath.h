#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPatrolPath.generated.h"

UCLASS()
class CPORTFOLIO_API ACPatrolPath : public AActor
{
	GENERATED_BODY()



private:
	UPROPERTY(EditAnywhere, Category = "Loop")
		bool bLoop;

	UPROPERTY(EditAnywhere, Category = "Path")
		int32 Index;

	UPROPERTY(EditAnywhere, Category = "Path")
		bool bReverse;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class USplineComponent* Spline;

	UPROPERTY(VisibleDefaultsOnly)
		class UTextRenderComponent* Text;


public:
	FORCEINLINE class USplineComponent* GetSpline() { return Spline; }



public:	
	ACPatrolPath();
	void OnConstruction(const FTransform& Transform) override; //에디터에서 값이 변경되면 호출되는 함수



protected:
	virtual void BeginPlay() override;


public:
	FVector GetMoveTo();
	void UpdateIndex();


};
