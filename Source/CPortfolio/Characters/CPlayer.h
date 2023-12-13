#pragma once

#include <NiagaraSystem.h>

#include "CoreMinimal.h"
#include "ICharacter.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Parkour/CParkourComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Components/CSubStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CWeaponStructures.h"
#include "CPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponHidden);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponVisible);

UCLASS()
class CPORTFOLIO_API ACPlayer
	: public ACharacter, public IICharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()


	
private:	//TeamID
	UPROPERTY(EditDefaultsOnly, Category = "Team")
		FGenericTeamId TeamID = FGenericTeamId(1);
	//

private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;


private:
	UPROPERTY(VisibleAnywhere)
		class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleAnywhere)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere)
		class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
		class UCSubStateComponent* SubState;

	UPROPERTY(VisibleDefaultsOnly)
		class UCParkourComponent* Parkour;

	UPROPERTY(VisibleDefaultsOnly)
		class UCTargetingComponent* Targeting;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCDefendComponent* Defend;


	UPROPERTY(VisibleDefaultsOnly)
		class UCAirComboComponent* AirCombo;



//parkour용
private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* ArrowGroup;

	UPROPERTY(VisibleDefaultsOnly)
		class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];

	//Dash
private:
	UPROPERTY(VisibleAnywhere)
		UNiagaraComponent* Niagara;

	UPROPERTY(EditAnywhere, Category = "Dash")
		UNiagaraSystem* DashEffect;

	UPROPERTY(EditAnywhere)
		UAnimMontage* LandedMontage;

	UPROPERTY(EditAnywhere)
		UAnimMontage* TestMontage;

	//zoom용
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCZoomComponent* Zoom;

	//HP bar
#if WITH_EDITOR	
private:
	UPROPERTY(VisibleDefaultsOnly)
		TSubclassOf<class UCUserWidget_PlayerStatus> HpBar;

private:
	UPROPERTY(VisibleDefaultsOnly)
		TSubclassOf<class UCUserWidget_SkillMenu> SkillMenu;

	UPROPERTY(VisibleDefaultsOnly)
		TSubclassOf<class UCUserWidget_SkillButton> Skill;


public:
	UPROPERTY(VisibleDefaultsOnly)
		TSubclassOf<class UCUserWidget_WeaponMenu> Menu;


#endif




public:
	FORCEINLINE bool& CanParrying() { return bCanParrying; }
	FORCEINLINE bool& AvoidAttack() { return bAvoidAttack; }
	FORCEINLINE bool& Launch() { return bLaunch; }
	FORCEINLINE bool& Fall()  { return bFall; }
	FORCEINLINE void CanParrying(bool b) { bCanParrying = b; }
	FORCEINLINE void AvoidAttack(bool b) { bAvoidAttack = b; }
	FORCEINLINE void Launch(bool b) { bLaunch = b; }

	FORCEINLINE EWeaponType GetCurrentWeapon() { return Weapon->GetWeaponType(); }

public:
	ACPlayer();


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
		void OnSubStateTypeChanged(ESubStateType InPrevType, ESubStateType InNewType);

public:
	FWeaponHidden OnWeaponHidden;
	FWeaponVisible OnWeaponVisible;

public:
	//무기 없을때 오른쪽 버튼 누르면 파쿠르 수행
	void On_RightButton();
	void Off_RightButton();

private:
	void OnAvoid();
	void Rise();
	void Test();
	void Faint();
	void Jump() override;

private:
	void Dash();
	void End_Dash();

private:
	void Roll();
	void Avoid();
	void AirMode();
	void AirComboLaunch();
	void Execute();
	void PlayExecute();

public:
	void End_Roll() override;
	void End_AirMode();

public:
	void Landed(const FHitResult& Hit) override;

private:
	virtual void Hitted();
	virtual void End_Hitted() override;

public:
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	ACharacter* Attacker() { return Damage.Character; }

private:
//활 에서 사용하는 zoom을 배제하기 위해서 zoomComponent에서 하지 않고 별도로 뺀다
	void SetZooming(float InValue);

public:
	void ShowMenu();
	void HideMenu();

public:
	//TeamID 세팅으로 Enemy에서 player을 적으로 감지할 수 있게 해준다
	void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	FGenericTeamId GetGenericTeamId() const override { return TeamID; }

private:	
	struct FDamageData	//CPlayer 내부적으로 사용할 구조체 - 직렬화 필요 없다
	{
		float Power;
		class ACharacter* Character;
		class AActor* Causer;

		struct FActionDamageEvent* Event;

	}Damage;

private:
	bool bCanParrying = false;
	bool bAvoidAttack = false;

private:
	FVector launchDirection;
	bool bLaunch = false;
	bool bFall = false;
	bool bExecute = false;

private:
	bool bCheck = false;	 // limit height 용
	float height = 0;
	int JumpCount = 0;
	bool bCanJump = true;
private:
	class ACEnemy* enemy;

private:
	UUserWidget* Widget;

private:
	UUserWidget* SkillWidget;

public:
	UCUserWidget_WeaponMenu* weaponMenu;
};


