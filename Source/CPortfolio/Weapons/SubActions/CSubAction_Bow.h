#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Components/TimelineComponent.h"
#include "CSubAction_Bow.generated.h"

USTRUCT()
struct FAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float TargetArmLength = 100;

	UPROPERTY(EditAnywhere)
		FVector SocketOffset = FVector(0, 30, 10);

	UPROPERTY(EditAnywhere)
		bool bEnableCameraLeg;

	UPROPERTY(EditAnywhere)
		FVector CameraLocation;


};


UCLASS(Blueprintable)
class CPORTFOLIO_API UCSubAction_Bow : public UCSubAction
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere, Category = "Aiming")
		class UCurveVector* Curve;

	UPROPERTY(EditAnywhere, Category = "Aiming")
		FAimData AimData;

	UPROPERTY(EditAnywhere, Category = "Aiming")
		float AimingSpeed = 200;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UCUserWidget_CrossHair> CrossHairClass;


public:
	UCSubAction_Bow();


public:
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;

public:
	void Tick_Implementation(float InDeltaTime) override;

private:
	UFUNCTION()
		void OnAiming(FVector Output);

public:
	virtual void Pressed() override;
	virtual void Released() override;



private:
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;


private:
	FAimData OriginData;

private:
	FTimeline Timeline;

private:
	float* Bend;

	//UI _ CrossHair
protected:
	class UCUserWidget_CrossHair* CrossHair;
};
