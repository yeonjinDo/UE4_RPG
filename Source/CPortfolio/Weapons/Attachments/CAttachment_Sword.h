#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttachment.h"
#include "CAttachment_Sword.generated.h"

UCLASS()
class CPORTFOLIO_API ACAttachment_Sword : public ACAttachment
{
	GENERATED_BODY()


private:
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* Capsule;


public:
	ACAttachment_Sword();


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


};
