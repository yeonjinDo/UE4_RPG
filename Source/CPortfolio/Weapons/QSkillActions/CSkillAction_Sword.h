#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSkillAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CSkillAction_Sword.generated.h"


UCLASS(Blueprintable)
class CPORTFOLIO_API UCSkillAction_Sword : public UCSkillAction
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACAttachment_Shield> AttachmentClass;


private:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FDoActionData ActionData;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FHitData HitData;


private:
	UFUNCTION()
		void OnCoolDown();

public:
	UCSkillAction_Sword();

public:
	void BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction) override;
	void Tick_Implementation(float InDeltaTime) override;

public:
	void Pressed() override;

	void Begin_SkillAction_Implementation() override;
	void End_SkillAction_Implementation() override;

	

private:
	class ACAttachment_Shield* shield;
	class ACAttachment* attachment;

	FTimerHandle TimerHandle;

};
