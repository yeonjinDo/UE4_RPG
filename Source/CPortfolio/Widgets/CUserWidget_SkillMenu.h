#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CWeaponComponent.h"
#include "CUserWidget_SkillMenu.generated.h"

UCLASS()
class CPORTFOLIO_API UCUserWidget_SkillMenu : public UUserWidget
{
	GENERATED_BODY()

private:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


public:
	UFUNCTION()
		//void SetSkil(EWeaponType InPrevType, EWeaponType InNewType);
		void SetSkill(int InSkillNum);



private:
	TArray<UTexture2D*> Textures;
	class UGridPanel* Grid;
	
	TArray<class UCUserWidget_SkillButton* > Buttons;

	class ACPlayer* player;


private:
	UCWeaponComponent* weapon;
	float CurrentTime = 0;
	float CoolTime = 5;
	UCUserWidget_SkillButton* Button;

	int SkillNum = 10;

};
