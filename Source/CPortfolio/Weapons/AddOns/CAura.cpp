#include "Weapons/AddOns/CAura.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"



ACAura::ACAura()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara", Root);
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Root);
	
	//NiagaraSystem'/Game/FX/sA_Megapack_v1/sA_StylizedAttacksPack/FX/NiagaraSystems/NS_AOE_Attack_2.NS_AOE_Attack_2'
	//NiagaraSystem'/Game/sA_StylizedSwordSet/Fx/NS_Ulti_lv1.NS_Ulti_lv1'

	UNiagaraSystem* niagara;
	CHelpers::GetAsset<UNiagaraSystem>(&niagara, "NiagaraSystem'/Game/sA_StylizedSwordSet/Fx/NS_Ulti_lv1.NS_Ulti_lv1'");
	Niagara->SetAsset(niagara);
}

void ACAura::BeginPlay()
{
	Super::BeginPlay();

	Niagara->SetNiagaraVariableObject("Mesh_Scale", this);
	Niagara->OnSystemFinished.AddDynamic(this, &ACAura::OnSystemFinished);


	Box->OnComponentBeginOverlap.AddDynamic(this, &ACAura::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACAura::OnComponentEndOverlap);

	FTimerDelegate delegate = FTimerDelegate::CreateLambda([&]()
	{	//중간에 빠질수도 있어서 거꾸로 돌려야한다
		for (int32 i = Hitted.Num() - 1; i >= 0; i--)
			HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[i]);
	});

	GetOwner()->GetWorld()->GetTimerManager().SetTimer(TimerHandle, delegate, DamageInterval, true, 0);

}

void ACAura::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem)
{
	//INiagaraParticleCallbackHandler::ReceiveParticleData_Implementation(Data, NiagaraSystem);

	Box->SetRelativeScale3D(Data[0].Position);

	//박스가 플레이어 중심으로 앞뒤로 늘어나는게 아니라 플레이어를 원점으로 앞으로 주욱 늘어나게하는거
	FVector location = Box->GetScaledBoxExtent();
	location.Y = 0;

	Box->SetRelativeLocation(location);

}


void ACAura::OnSystemFinished(UNiagaraComponent* PSystem)
{
	Destroy();
}

void ACAura::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
		Hitted.AddUnique(character);
}

void ACAura::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
		Hitted.Remove(character);
}
