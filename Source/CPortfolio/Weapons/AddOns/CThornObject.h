#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraDataInterfaceExport.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapons/CWeaponStructures.h"
#include "CThornObject.generated.h"

UCLASS()
class CPORTFOLIO_API ACThornObject : public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()


private:
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		FHitData HitData;

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
		class UStaticMesh* NiagaraMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
		FRotator NiagaraMeshRotation;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
		class UFXSystemAsset* CollisionEffect;	//충돌했을때 이펙트

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
		FTransform CollisionEffectTransform;

	UPROPERTY(EditAnywhere, Category = "Trace")
		TEnumAsByte<EDrawDebugTrace::Type> DebugType;


private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class UNiagaraComponent* Niagara;




public:	
	ACThornObject();

protected:
	virtual void BeginPlay() override;


public:
	void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem);


private:
	UFUNCTION()
		void OnSystemFinished(class UNiagaraComponent* PSystem);

private:
	FVector BoxExtent;


};
