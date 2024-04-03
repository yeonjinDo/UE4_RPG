#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Bow.generated.h"


UCLASS(Blueprintable)
class CPORTFOLIO_API UCDoAction_Bow : public UCDoAction
{
	GENERATED_BODY()


private:
	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
		TSubclassOf<class ACArrow> ArrowClass;

public:
	FORCEINLINE FVector& GetSpawnLocation() { return spawnLocation; }
	FORCEINLINE FRotator& GetSpawnRotation() { return spawnRotation; }

public:
	UCDoAction_Bow();

	void BeginPlay
	(
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		class ACharacter* InOwner,
		const TArray<FDoActionData>& InDoActionData,
		const TArray<FHitData>& InHitData
	) override;

	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

	void OnBeginEquip() override;
	void OnUnequip() override;

	void Tick(float InDeltaTime) override;


public:
	void End_BowString();


private:
	void CreateArrow();
	class ACArrow* GetAttachedArrow();

private:
	UFUNCTION()
		void OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter);

	UFUNCTION()
		void OnArrowEndPlay(class ACArrow* InDestroyer);




private:
	class UPoseableMeshComponent* PoseableMesh;
	class USkeletalMeshComponent* SkeletalMesh;


private:
	bool bAttachedString = true;	//활 줄이 손에 붙어있는가
	FVector OriginLocation;

	float* Bend;

private:
	const bool* bEquipped;

private:
	TArray<class ACArrow*> Arrows;	//현재 생성한 화살 목록


	float ScaleForwardVector = 15000;
	FVector spawnLocation;
	FRotator spawnRotation;

};
