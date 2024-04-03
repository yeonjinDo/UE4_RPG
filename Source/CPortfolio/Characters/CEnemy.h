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
	struct FDamageData	//CEnemy ���������� ����� ����ü - ����ȭ �ʿ� ����
	{
		float Power;
		class ACharacter* Character;
		class AActor* Causer;

		struct FActionDamageEvent* Event;

	}Damage;

	FTimerHandle RestoreColor_TimerHandle;	//���� �ð� �� �� �ǵ����� Ÿ�̸�



protected:
	ACharacter* Player;
	TArray<ACharacter*> hittedCharacters;
	FRotator LookAtRotation;
	FRotator TargetRotation;

#if WITH_EDITOR
private:	//�̷��� �ϴϱ� ��װ� �������÷��Ϳ� �������� ������� �������ͷ�,,
	//�ʿ��Ҷ����� �����ؼ� ����Ѵ� �����̺��Ʈ������ �Ź� ĳ�����ϴ� ������ �־���. ����

	//UUserWidget* DamageWidget;
	//UCUserWidget_Damage* damageWidget;
#endif	
};



