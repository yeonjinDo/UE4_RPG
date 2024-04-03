#include "Weapons/AddOns/CThornObject.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"


ACThornObject::ACThornObject()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara", Root);
}

void ACThornObject::BeginPlay()
{
	Super::BeginPlay();

	Niagara->SetNiagaraVariableObject("Collision", this);
	Niagara->OnSystemFinished.AddDynamic(this, &ACThornObject::OnSystemFinished);

	if(!!NiagaraMesh)
		BoxExtent = NiagaraMesh->GetBoundingBox().GetExtent();	//getboundingbox = �浹�Ǵ� �ڽ�

}


void ACThornObject::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data,
	UNiagaraSystem* NiagaraSystem)								//��ƼŬ ���� ������ 25���� �迭
{
	
	//INiagaraParticleCallbackHandler::ReceiveParticleData_Implementation(Data, NiagaraSystem);

	CheckFalse(Data.Num() > 0);

	static TArray<AActor*> ignores;	//�Ź� �Ҵ���� �ʰ� �ѹ� �Ҵ�� �Ŀ� ��� ����ϵ��� static�� ���δ�
	ignores.AddUnique(GetOwner());

	static FHitResult hitResult;
	for (int32 i = Data.Num()-1; i >= 0; i--)
	{
		FVector position = Data[i].Position + GetActorLocation();
		FVector scale = Data[i].Velocity * BoxExtent;


		UKismetSystemLibrary::BoxTraceSingleByProfile(GetWorld(), position, position, scale, NiagaraMeshRotation, "Pawn", 
														false, ignores, DebugType, hitResult, true);


		if (hitResult.bBlockingHit)
		{
			float height = 0;

			if (!!CollisionEffect)
			{

				//�浹�� ���͸� ĳ���ͷ� ĳ���� ->  ��(ĳ������)�� ���
				ACharacter* hittedCharacter = Cast<ACharacter>(hitResult.GetActor());
				//hittedCharacter�� ĸ�� height�� ���Ѵ� = height
				if (!!hittedCharacter)
					height = hittedCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.0f;

				FTransform transform = CollisionEffectTransform;

				//ĳ���� �Ӹ� ���� �¾������� �ٴڿ��� ����Ʈ �������Ѵ�
				FVector location = FVector(hitResult.Location.X, hitResult.Location.Y, hitResult.Location.Z - height);
				transform.AddToTranslation(location);

				CHelpers::PlayEffect(GetWorld(), CollisionEffect, transform);
			}


			ACharacter* character = Cast<ACharacter>(hitResult.GetActor());
			if (!!character)
				HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, character);
		}
	}

}

void ACThornObject::OnSystemFinished(UNiagaraComponent* PSystem)
{
	Destroy();
}

