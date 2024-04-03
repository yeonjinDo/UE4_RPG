#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapons/CWeaponStructures.h"
#include "CSubAction_Sword.generated.h"

UCLASS(Blueprintable)
class CPORTFOLIO_API UCSubAction_Sword : public UCSubAction
{
	GENERATED_BODY()


private:
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		float Distance = 1000;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		float Speed = 200;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FDoActionData ActionData;


	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FHitData HitData;

private:
	UPROPERTY(EditAnywhere, Category = "Add-On")
		TSubclassOf<class ACGhostTrail> GhostTrailClass;

public:
	UCSubAction_Sword();

public :
	virtual void Pressed() override;
		    
	virtual void Begin_SubAction_Implementation() override;
	virtual void End_SubAction_Implementation() override;
	virtual void Tick_Implementation(float InDeltaTime) override;

	virtual float GetPercent() override;


private:
	UFUNCTION()
		void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);


private:
	UFUNCTION()
		void OnCoolDown();


private:
	bool bMoving;

	FVector Start;
	FVector End;

	TArray<class ACharacter*> Overlapped;
	TArray<class ACharacter*> Hitted;


private:
	class ACGhostTrail* GhostTrail;

	FTimerHandle TimerHandle;
};

