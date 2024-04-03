#pragma once

#include "CoreMinimal.h"
#include "CWeaponComponent.h"
#include "Components/ActorComponent.h"
#include "Components/CStateComponent.h"
#include "Engine/DataTable.h"
#include "CMontagesComponent.generated.h"

USTRUCT()
struct FMontagesData
	:public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EStateType Type;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;

};

USTRUCT()
struct FAvoidAttackData
	:public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EWeaponType Type;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CPORTFOLIO_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCMontagesComponent();

private:
	UPROPERTY(EditAnywhere, Category = "DataTables")
		UDataTable* DataTable;

	UPROPERTY(EditAnywhere, Category = "DataTables")
		UDataTable* AvoidAttackDataTable;

protected:
	virtual void BeginPlay() override;

public:
	void PlayBackStepMode();
	void PlayRightRollMode();
	void PlayLeftRollMode();
	void PlayAvoidMode();
	void PlayCounterMode();
	void PlayDefendMode();
	void PlayEndDefendMode();
	void PlayParryingMode();
	void PlayDeadMode();
	void PlayRiseMode();
	void PlayExecuteMode();
	void PlayJumpMode();
	void PlayDoubleJumpMode();
	void PlayHittedMode();
	void PlayTurnLeftMode();
	void PlayTurnRightMode();

private:
	void PlayAnimMontage(EWeaponType InType);
	void PlayAnimMontage(EStateType InType);
	void PlayAnimMontage(EStateType InType, FName InSectionName);

private:
	class ACharacter* OwnerCharacter;

	FMontagesData* Datas[(int32)EStateType::Max];
	FAvoidAttackData* AvoidAttackDatas[(int32)EWeaponType::Max];

};
