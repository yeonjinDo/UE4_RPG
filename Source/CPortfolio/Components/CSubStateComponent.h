#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSubStateComponent.generated.h"



UENUM()
enum class ESubStateType : uint8
{
	Ground,
	Dash,
	Fly,
	AirCombo,
	Fall,
	Faint,
	Max,
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSubStateTypeChanged, ESubStateType, InPrevType, ESubStateType, InNewType);




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPORTFOLIO_API UCSubStateComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	FORCEINLINE bool IsGroundMode()   { return Type == ESubStateType::Ground; }
	FORCEINLINE bool IsDashMode()     { return Type == ESubStateType::Dash; }
	FORCEINLINE bool IsFlyMode()	  { return Type == ESubStateType::Fly; }
	FORCEINLINE bool IsAirComboMode() { return Type == ESubStateType::AirCombo; }
	FORCEINLINE bool IsFallMode()	  { return Type == ESubStateType::Fall; }
	FORCEINLINE bool IsFaintMode()	  { return Type == ESubStateType::Faint; }

public:
	FORCEINLINE ESubStateType GetSubStateType() { return Type; }


public:	
	UCSubStateComponent();

protected:
	virtual void BeginPlay() override;


public:
	void SetGroundMode();
	void SetDashMode();
	void SetFlyMode();
	void SetAirComboMode();
	void SetFallMode();
	void SetFaintMode();


private:
	void ChangeType(ESubStateType InType);


public:
	FSubStateTypeChanged OnSubStateTypeChanged;


private:
	ESubStateType Type;

		
};
