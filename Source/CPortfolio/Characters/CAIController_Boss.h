#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController_Boss.generated.h"

UCLASS()
class CPORTFOLIO_API ACAIController_Boss : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class UAIPerceptionComponent* Perception;



public:
	ACAIController_Boss();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);


protected:
	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;

private:
	class UCAIBehaviorComponent* Behavior;

private:
	class ACEnemy_AI* Enemy;


private:	//감지 객체, 시야로
	class UAISenseConfig_Sight* Sight;

};
