#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSubAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSubActionCoolDown);

UCLASS(Abstract)	 //객체화 되면 안되기 때문에 Abstract를 붙여준다
class CPORTFOLIO_API UCSubAction : public UObject
{
	GENERATED_BODY()


public:
	FORCEINLINE bool GetInAction() { return bInAction; }

	FORCEINLINE void SetSkillNum(int InNumber) { SkillNum = InNumber; }
	FORCEINLINE int GetSkillNum() { return SkillNum; }

	FORCEINLINE float GetCoolTime() { return CoolTime; }


public:
	FSubActionCoolDown OnSubActionCoolDown;

public:
	UCSubAction();


public:
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction);

public:
	virtual void Pressed();
	virtual void Released();

	virtual void QPressed(){}

	virtual float GetPercent();

public:
	UFUNCTION(BlueprintNativeEvent)		//정의해둘테니, 필요할 때 블프에서 재정의해서 써라
		void Begin_SubAction();
	virtual void Begin_SubAction_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)		
		void End_SubAction();
	virtual void End_SubAction_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void Tick(float InDeltaTime);
	virtual void Tick_Implementation(float InDeltaTime) {}

	

protected:
	bool bInAction;

	class ACharacter*   Owner;
	class ACAttachment* Attachment;
	class UCDoAction*   DoAction;

	class UCStateComponent*    State;
	class UCMovementComponent* Movement;

	int SkillNum = 0;


	float CoolTime = 15.0f;
	float CurrentTime = CoolTime;

	bool bCoolTime = false;
};
