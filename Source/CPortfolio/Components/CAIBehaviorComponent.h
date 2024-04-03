#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAIBehaviorComponent.generated.h"


//AI Çàµ¿ »óÅÂ Á¤ÀÇ
UENUM(BlueprintType)
enum class EAIStateType : uint8
{
	Wait = 0,
	Approach,
	Action,
	Patrol,
	Hitted,
	Parrying,
	Avoid,
	Dead,
	Strafe,
	Dash,
	Max,
};

UENUM(BlueprintType)
enum class EBossActionType : uint8
{
	Ready = 0,
	Idle,
	Action,
	Dash,
	Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChanged, EAIStateType, InPrevType, EAIStateType, InNewType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBossStateTypeChanged, EBossActionType, InPrevType, EBossActionType, InNewType);



UCLASS()
class CPORTFOLIO_API UCAIBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Key")
		FName AIStateTypeKey = "AIState";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName TargetKey      = "Target";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName PatrolLocationKey = "Patrol_Location";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName AvoidLocationKey = "Avoid_Location";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName BossPhaseKey = "BossPhase";

public:
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }
	//FORCEINLINE FName& GetAIState() {	return AIStateTypeKey;	}

public:	
	UCAIBehaviorComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	bool IsWaitMode();
	bool IsApproachMode();
	bool IsActionMode();
	bool IsPatrolMode();
	bool IsHittedMode();
	bool IsParryingMode();
	bool IsAvoidMode();
	bool IsDeadMode();
	bool IsStrafeMode();

public:
	bool IsIdle();
	bool IsReady();
	bool IsAction();
	bool IsDash();

public:
	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetHittedMode();
	void SetParryingMode();
	void SetAvoidMode();
	void SetDeadMode();
	void SetStrafeMode();

public:
	void SetIdle();
	void SetReady();
	void SetAction();
	void SetDash();



public:
	FVector GetPatrolLocation();
	void SetPatrolLocation(const FVector& InLocation);

public:
	FVector GetAvoidLocation();

public:	//ÇÏ´Ã¿¡¼­ ºÒ²ÉÀÌ ¶³¾îÁ®¿ä ·£´ý Æ÷ÀÎÆ® ÁÂÇ¥
	TArray<FVector> GetRandomPoints();
	void SetRandomPoints(const TArray<FVector>& InLocations);

	FTransform GetNotifyTransform();
	void SetNotifyTransform(const FTransform& InTransform);

public:
	EAIStateType GetType();
	EBossActionType GetActionType();

public:
	ACharacter* GetTarget();


private:
	void ChangeType(EAIStateType InType);
	void ChangePhase(EBossActionType InType);



public:
	FAIStateTypeChanged OnAIStateTypeChanged;
	FBossStateTypeChanged OnBossStateTypeChanged;

private:
	class UBlackboardComponent* Blackboard;


private:
	TArray<FVector> RandomLocations;
	FTransform NotifyTransform;

	
};
