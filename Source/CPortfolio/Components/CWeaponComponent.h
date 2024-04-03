#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponHidden);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponVisible);


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Fist, Sword, Hammer, Warp, Around, Bow, Max,
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);
//무기 바뀔 때 자동으로 콜

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPORTFOLIO_API  UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "DataAsset")
		class UCWeaponAsset* DataAssets[(int32)EWeaponType::Max];

private:
	UPROPERTY()
		class UCWeaponData* Datas[(int32)EWeaponType::Max];

public:
	FWeaponHidden OnWeaponHidden;
	FWeaponVisible OnWeaponVisible;


public:
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
	FORCEINLINE bool IsFistMode()    { return Type == EWeaponType::Fist; }
	FORCEINLINE bool IsSwordMode()   { return Type == EWeaponType::Sword; }
	FORCEINLINE bool IsHammerMode()  { return Type == EWeaponType::Hammer; }
	FORCEINLINE bool IsWarpMode()    { return Type == EWeaponType::Warp; }
	FORCEINLINE bool IsAroundMode()  { return Type == EWeaponType::Around; }
	FORCEINLINE bool IsBowMode()     { return Type == EWeaponType::Bow; }

public:
	FORCEINLINE EWeaponType GetWeaponType() { return Type; }

public:	
	UCWeaponComponent();


protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	bool IsIdleMode();


public:
	class ACAttachment*  GetAttachment();
	class ACAttachment*  GetSubAttachment();
	class UCEquipment*   GetEquipment();
	class UCDoAction*    GetDoAction();
	class UCSubAction*   GetSubAction();
	class UCSkillAction* GetSkillAction();


public:
	void SetUnarmedMode();
	void SetFistMode();
	void SetSwordMode();
	void SetHammerMode();
	void SetWarpMode();
	void SetAroundMode();
	void SetBowMode();
	//void SetDefendMode();
	void SetPrevMode();

	void DoAction();

public:
	UFUNCTION(BlueprintCallable)
		void SubAction_Pressed();

	UFUNCTION(BlueprintCallable)
		void SubAction_Released();

	UFUNCTION(BlueprintCallable)
		void SkillAction_Pressed();

private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);


public:
	FWeaponTypeChanged OnWeaponTypeChange;

private:
	class ACharacter* OwnerCharacter;

	EWeaponType Type     = EWeaponType::Max;
	EWeaponType PrevType = EWeaponType::Max;

	class UCSubStateComponent* SubState;

};
