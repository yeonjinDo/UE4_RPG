#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentBeginCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentEndCollision);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, AActor*, InAttackCauser, class ACharacter*, InOther);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class ACharacter*, InOther);



UCLASS()
class CPORTFOLIO_API ACAttachment : public AActor
{
	GENERATED_BODY()


protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class USceneComponent* Root;


public:	
	ACAttachment();

protected:
	virtual void BeginPlay() override;


public:
	UFUNCTION(BlueprintNativeEvent)
		void OnBeginEquip();
	virtual void OnBeginEquip_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void OnEndEquip();
	virtual void OnEndEquip_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void OnUnequip();
	virtual void OnUnequip_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void OnHidden();
	virtual void OnHidden_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void OnVisible();
	virtual void OnVisible_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void OnDestroy();
	virtual void OnDestroy_Implementation() {}


protected:
	UFUNCTION(BlueprintCallable, Category = "Attach")
		void AttachTo(FName InSocketName);

	UFUNCTION(BlueprintCallable, Category = "Attach")
		void AttachToCollision(FName InSocketName);

public:
	UFUNCTION(BlueprintCallable, Category = "Throw")
		void Throw();

public:
	void OnCollisions();
	void OffCollisions();

	FORCEINLINE TArray<class ACharacter*> HittedCharacters() { return Hitted; }


private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	FAttachmentBeginCollision OnAttachmentBeginCollision;
	FAttachmentEndCollision   OnAttachmentEndCollision;

	FAttachmentBeginOverlap OnAttachmentBeginOverlap;
	FAttachmentEndOverlap   OnAttachmentEndOverlap;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Game")
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Game")
		TArray<class UShapeComponent*> Collisions;


private:
	TArray<class ACharacter*> Hitted;

};