#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSkillAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSkillActionCoolDown);

UCLASS(Abstract)
class CPORTFOLIO_API UCSkillAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetInAction() { return bInAction; }

	FORCEINLINE void SetSkillNum(int InNumber) { SkillNum = InNumber; }
	FORCEINLINE int GetSkillNum() { return SkillNum; }

	FORCEINLINE float GetCoolTime() { return CoolTime; }

	FORCEINLINE bool GetCanSubAction() { return bCanSubAction; }

public:
	FSkillActionCoolDown OnSkillActionCoolDown;

public:
	UCSkillAction();


public:
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction);

public:
	virtual void Pressed();
	virtual void Released();

	float GetPercent();

public:
	UFUNCTION(BlueprintNativeEvent)		//정의해둘테니, 필요할 때 블프에서 재정의해서 써라
		void Begin_SkillAction();
	virtual void Begin_SkillAction_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void End_SkillAction();
	virtual void End_SkillAction_Implementation() {}

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

	bool bCanSubAction = true;
};
