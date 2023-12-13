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

public:
	class UProgressBar* CoolTime;


private:
	float CurrentTime = 5;
	bool bStartTick = false;
};
