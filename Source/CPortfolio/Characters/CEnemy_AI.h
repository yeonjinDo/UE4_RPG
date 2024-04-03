#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDead);


UCLASS()
class CPORTFOLIO_API ACEnemy_AI : public ACEnemy, public IGenericTeamAgentInterface
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

private:
	UPROPERTY(EditAnywhere, Category = "Patrol")
		class ACPatrolPath* PatrolPath;


#if WITH_EDITOR	//위젯용
private:	
	UPROPERTY(EditDefaultsOnly, Category = "Label")
		float LabelViewAmount = 3000.0f;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* LabelWidget;

#endif


protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleDefaultsOnly)
		class UCAIBehaviorComponent* Behavior;


public:
	FORCEINLINE uint8 GetTeamID() { return GetGenericTeamId(); }
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE class ACPatrolPath* GetPatrolPath() { return PatrolPath; }


public:
	ACEnemy_AI();


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	FDead OnDead;

protected:
	virtual void Hitted() override;

public:
	virtual void End_Hitted() override;
	virtual void End_Dead() override;



#if WITH_EDITOR
private:	//Label 크기 재조정
	void UpdateLabelRenderScale();

protected:
	class UCUserWidget_Label* label;
#endif


};
