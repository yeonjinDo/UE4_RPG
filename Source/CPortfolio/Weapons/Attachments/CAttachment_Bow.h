#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttachment.h"
#include "CAttachment_Bow.generated.h"

UCLASS()
class CPORTFOLIO_API ACAttachment_Bow : public ACAttachment
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class UPoseableMeshComponent* Mesh;

private:
	UPROPERTY(EditDefaultsOnly, Category = "View")
		FVector2D ViewPitchRange = FVector2D(-40, +30);

private:
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere)
		class UPoseableMeshComponent* PoseableMesh;


public:
	ACAttachment_Bow();


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	float* GetBend();

public:
	void OnBeginEquip_Implementation() override;
	void OnUnequip_Implementation() override;

	void OnHidden_Implementation() override;
	void OnVisible_Implementation() override;

	void OnDestroy_Implementation() override;

private:
	FVector2D OriginViewPitchRange; //허리 숙이고 높일 수 있는 각도(위아래로) 제한

	class UCSubStateComponent* SubState;
};

