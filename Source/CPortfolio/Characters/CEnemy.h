#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/ICharacter.h"
#include "Components/CStateComponent.h"
#include "Components/CSubStateComponent.h"
#include "CEnemy.generated.h"

UCLASS()
class CPORTFOLIO_API ACEnemy : public ACharacter, public IICharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Color")
		FLinearColor OriginColor = FLinearColor::White;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UCMovementComponent* Movement;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
		class UCSubStateComponent* SubState;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCDefendComponent* Defend;

	UPROPERTY(VisibleDefaultsOnly)
		class UCAirComboComponent* AirCombo;

#if WITH_EDITOR	
protected:
	UPROPERTY(VisibleDefaultsOnly)
		TSubclassOf<class UCUserWidget_Damage> DamageUI;
	
#endif

private:
	UFUNCTION()
		void RestoreColor();

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


public:
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void Hitted();
	virtual void End_Hitted() override;


protected:
	void Dead();

public:
	void End_Dead() override;


protected:	
	struct FDamageData	//CEnemy 내부적으로 사용할 구조체 - 직렬화 필요 없다
	{
		float Power;
		class ACharacter* Character;
		class AActor* Causer;

		struct FActionDamageEvent* Event;

	}Damage;

	FTimerHandle RestoreColor_TimerHandle;	//일정 시간 후 색 되돌려줄 타이머



protected:
	ACharacter* Player;
	TArray<ACharacter*> hittedCharacters;
	FRotator LookAtRotation;
	FRotator TargetRotation;

#if WITH_EDITOR
private:	//이렇게 하니까 얘네가 가비지컬렉터에 가버려서 사라진다 널포인터로,,
	//필요할때마다 생성해서 써야한다 비헤이비어트리에서 매번 캐스팅하는 이유가 있었네. ㅋㅋ

	//UUserWidget* DamageWidget;
	//UCUserWidget_Damage* damageWidget;
#endif	
};



