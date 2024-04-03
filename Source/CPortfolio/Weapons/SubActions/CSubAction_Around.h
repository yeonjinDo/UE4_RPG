#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CSubAction_Around.generated.h"

UCLASS(Blueprintable)
class CPORTFOLIO_API UCSubAction_Around : public UCSubAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "FallObject")
		TSubclassOf<class ACThorn_Floor> ObjectClass;

	UPROPERTY(EditDefaultsOnly, Category = "FallObject")
		FVector ObjectLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Notify")
		TSubclassOf<class ACNotify_Around> NotifyClass;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FDoActionData ActionData;

	UPROPERTY(EditAnywhere)
		int32 Speed = 1;

	UFUNCTION()
		void OnCoolDown();


public:
	UCSubAction_Around();

public:
	virtual void Pressed() override;
	virtual void Released() override;

	virtual void Begin_SubAction_Implementation() override;
	virtual void End_SubAction_Implementation() override;

private:
	class ACNotify_Around* notifyClass;

	FVector notifyLocation;
	FRotator notifyRotation;

	FTimerHandle TimerHandle;
};
