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
		BoxExtent = NiagaraMesh->GetBoundingBox().GetExtent();	//getboundingbox = 충돌되는 박스

}


void ACThornObject::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data,
	UNiagaraSystem* NiagaraSystem)								//파티클 각각 스폰된 25개의 배열
{
	
	//INiagaraParticleCallbackHandler::ReceiveParticleData_Implementation(Data, NiagaraSystem);

	CheckFalse(Data.Num() > 0);

	static TArray<AActor*> ignores;	//매번 할당되지 않고 한번 할당된 후에 계속 사용하도록 static을 붙인다
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

				//충돌된 액터를 캐릭터로 캐스팅 ->  적(캐릭터형)일 경우
				ACharacter* hittedCharacter = Cast<ACharacter>(hitResult.GetActor());
				//hittedCharacter의 캡슐 height를 구한다 = height
				if (!!hittedCharacter)
					height = hittedCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.0f;

				FTransform transform = CollisionEffectTransform;

				//캐릭터 머리 위에 맞았을때도 바닥에서 이펙트 터지게한다
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

