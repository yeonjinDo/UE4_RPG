#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CSubAction_Hammer.generated.h"

UCLASS(Blueprintable)
class CPORTFOLIO_API UCSubAction_Hammer : public UCSubAction
{
	GENERATED_BODY()


private:
	UPROPERTY(EditDefaultsOnly, Category = "Aura")
		TSubclassOf<class ACAura> AuraClass;

	UPROPERTY(EditDefaultsOnly, Category = "Aura")
		FVector AuraLocation;


private:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FDoActionData ActionData;

	UPROPERTY(EditDefaultsOnly, Category = "Add-On")
		TSubclassOf<class ACGhostTrail> GhostTrailClass;

private:
	UFUNCTION()
		void OnCoolDown();

private:
	UCSubAction_Hammer();

public:
	virtual void Pressed() override;
	
	virtual void Begin_SubAction_Implementation() override;
	virtual void End_SubAction_Implementation() override;

private:
	class ACGhostTrail* GhostTrail;
	
	FTimerHandle TimerHandle;
};
