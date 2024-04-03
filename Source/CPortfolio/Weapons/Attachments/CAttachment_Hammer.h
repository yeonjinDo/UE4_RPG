#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttachment.h"
#include "CAttachment_Hammer.generated.h"

UCLASS()
class CPORTFOLIO_API ACAttachment_Hammer : public ACAttachment
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* Skeletal;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* Cap;


public:
	ACAttachment_Hammer();


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
	bool bEquip;
};
