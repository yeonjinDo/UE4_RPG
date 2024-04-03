#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_WeaponButton.generated.h"

UCLASS()
class CPORTFOLIO_API UCUserWidget_WeaponButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeOnInitialized() override;

public:
	UFUNCTION()
		void OnClicked();

	UFUNCTION()
		void OnHovered();

	UFUNCTION()
		void OnUnhovered();


private:
	class UBorder* Border;
	class UButton* Button;

public:
	class UBorder* Select;
	class UButton* ImageButton;


};
