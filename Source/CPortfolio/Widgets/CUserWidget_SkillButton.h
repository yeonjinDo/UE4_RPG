#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_SkillButton.generated.h"

UCLASS()
class CPORTFOLIO_API UCUserWidget_SkillButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeOnInitialized() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	FORCEINLINE void SetCurrentTime(float InTime) { CurrentTime = InTime; }
	FORCEINLINE float GetCurrentTime() { return CurrentTime; }

	FORCEINLINE bool GetStartTick() { return bStartTick; }
	FORCEINLINE void SetStartTick(bool b) { bStartTick = b; }
	FORCEINLINE void SetMaxCoolTime(float time) { MaxCoolTime = time; }

public:
	class UProgressBar* CoolTime;
	class ACPlayer* player;


private:
	float CurrentTime = 5;
	float MaxCoolTime = 5;
	bool bStartTick = false;

};
