#pragma once

#include "CoreMinimal.h"
#include "Components/CStateComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CWeaponStructures.h"
#include "CAttachment_Shield.generated.h"

UCLASS()
class CPORTFOLIO_API ACAttachment_Shield : public ACAttachment
{
	GENERATED_BODY()


private:
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* Box;


	UPROPERTY(EditAnywhere, Category = "Projectile")
		float Speed = 500;

	UPROPERTY(EditAnywhere, Category = "CatchShield")
		UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, Category = "CatchShield")
		float PlayRatio = 1;

	UPROPERTY(EditAnywhere, Category = "CatchShield")
		FName SectionName;

	UPROPERTY(EditAnywhere, Category = "Trace")
		TEnumAsByte<EDrawDebugTrace::Type> DebugType;

	UPROPERTY(EditDefaultsOnly, Category = "HitData")
		FHitData HitData;


public:
	ACAttachment_Shield();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void OnBeginEquip_Implementation() override;
	void OnUnequip_Implementation() override;

	void OnHidden_Implementation() override;
	void OnVisible_Implementation() override;

	void OnDestroy_Implementation() override;

private:
	UFUNCTION(BlueprintCallable)
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


public:
	void Throw(const FVector& InForward);
	void Catch();


private:
	TArray<class ACharacter*> Hitted;
	ACharacter* HittedCharacter;



private:
	FHitResult HitResults;

private:
	FVector angle;
	bool bThrow = false;
	bool bDefendMode = false;

	class UCStateComponent*    State;
	class UCSubStateComponent*    SubState;
	class UCWeaponComponent*   Weapon;
};
