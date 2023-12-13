#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM()
enum class EStateType : uint8
{
	Idle = 0,
	BackStep,
	RightRoll,
	LeftRoll,
	Avoid,
	Counter,
	Equip,
	Hitted,
	Dead,
	Action,
	Defend,	//10
	Parrying,
	Rise,
	Execute,
	Jump,
	DoubleJump,
	Max
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CPORTFOLIO_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsIdleMode()     { return Type == EStateType::Idle; }
	FORCEINLINE bool IsBackstepMode() { return Type == EStateType::BackStep; }
	FORCEINLINE bool IsRightRollMode(){ return Type == EStateType::RightRoll; }
	FORCEINLINE bool IsLeftRollMode() { return Type == EStateType::LeftRoll; }
	FORCEINLINE bool IsAvoidMode()	  { return Type == EStateType::Avoid; }
	FORCEINLINE bool IsEquipMode()    { return Type == EStateType::Equip; }
	FORCEINLINE bool IsHittedMode()   { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsDeadMode()     { return Type == EStateType::Dead; }
	FORCEINLINE bool IsActionMode()   { return Type == EStateType::Action; }
	FORCEINLINE bool IsDefendMode()   { return Type == EStateType::Defend; }
	FORCEINLINE bool IsParryingMode() { return Type == EStateType::Parrying; }
	FORCEINLINE bool IsRiseMode()	  { return Type == EStateType::Rise; }
	FORCEINLINE bool IsExecuteMode()  { return Type == EStateType::Execute; }
	FORCEINLINE bool IsJumpMode()     { return Type == EStateType::Jump; }

	FORCEINLINE bool IsSubActionMode() { return bInSubActionMode; }

	FORCEINLINE EStateType& GetState() { return Type; }

public:
	UCStateComponent();

public:
	void SetIdleMode();
	void SetBackStepMode();
	void SetRightRollMode();
	void SetLeftRollMode();
	void SetAvoidMode();
	void SetEquipMode();
	void SetHittedMode();
	void SetDeadMode();
	void SetActionMode();
	void SetDefendMode();
	void SetParryingMode();
	void SetRiseMode();
	void SetExecuteMode();
	void SetJumpMode();


	void OnSubActionMode();
	void OffSubActionMode();


private:
	void ChangeType(EStateType InType);

protected:
	virtual void BeginPlay() override;

public:
	FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Type;


private:
	bool bInSubActionMode;

};
