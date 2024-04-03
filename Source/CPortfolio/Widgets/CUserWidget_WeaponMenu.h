#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
#include "CUserWidget_WeaponMenu.generated.h"

UCLASS()
class CPORTFOLIO_API UCUserWidget_WeaponMenu : public UUserWidget
{
	GENERATED_BODY()



private:
	virtual void NativeConstruct() override;


public:
	void OnClicked(FString InName);
	void OnHovered(FString InName);
	void OnUnhovered(FString InName);

private:
	void OnClicked_Break();
	void OnHovered_Break();
	void OnUnhovered_Break();


private:
	class UCUserWidget_WeaponButton* Button;
	class UGridPanel* Grid;

	TArray<UTexture2D*> Textures;
};
