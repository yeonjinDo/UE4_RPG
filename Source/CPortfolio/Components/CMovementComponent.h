#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMovementComponent.generated.h"

UENUM()
enum class ESpeedType : uint8
{
	Walk = 0,
	Run,
	Sprint,
	Dash,
	Frozen,
	Max
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CPORTFOLIO_API UCMovementComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
		float HorizontalLook = 45;

	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
		float VerticalLook = 45;

private:
	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed[(int32)ESpeedType::Max] = { 200, 600, 1000, 2500, 0 };

private:
	UPROPERTY(EditAnywhere, Category = "Turn")
		bool bTurnRight = false;

	UPROPERTY(EditAnywhere, Category = "Turn")
		bool bTurnLeft = false;

public:
	FORCEINLINE bool CanMove() { return bCanMove; }
	FORCEINLINE void Move()    { bCanMove = true; }
	FORCEINLINE void Stop()    { bCanMove = false; }

	FORCEINLINE float GetWalkSpeed()   { return Speed[(int32)ESpeedType::Walk]; }
	FORCEINLINE float GetRunSpeed()    { return Speed[(int32)ESpeedType::Run]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ESpeedType::Sprint]; }

	FORCEINLINE bool GetFixedCamera() { return bFixedCamera; }
	FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }

	//Warp _SubAction에서 캐릭터 기준 전방방향이 아니라
	//카메라 기준 전방방향으로 움직이도록
	FORCEINLINE void EnableTopViewCamera() { bTopViewCamera = true; }
	FORCEINLINE void DisableTopViewCamera() { bTopViewCamera = false; }

	FORCEINLINE bool TurnLeft() { return bTurnLeft; }
	FORCEINLINE bool TurnRight() { return bTurnRight; }

public:
	UCMovementComponent();

	void SetSpeed(ESpeedType InType);

	void OnSprint();
	void OnRun();
	void OnWalk();
	void OnDash(class UNiagaraSystem* DashEffect);
	void OnEndDash();

	void EnableControlRotation();
	void DisableControlRotation();

public:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);

public:
	FVector GetFutureLocation(ACharacter* InCharacter, float InTime);


protected:
	virtual void BeginPlay() override;

private:
	class ACharacter* OwnerCharacter;
	class UNiagaraComponent* Dash;

private:
	bool bCanMove = true;
	bool bFixedCamera;
	bool bTopViewCamera;


};
