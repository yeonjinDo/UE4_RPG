#include "Weapons/AddOns/CGhostTrail.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"



ACGhostTrail::ACGhostTrail()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &Mesh, "Mesh");
}

void ACGhostTrail::BeginPlay()
{
	Super::BeginPlay();

	//Dynamic Material 값 주기
	UMaterialInstanceConstant* material;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Materials/M_GhostTrail_Inst.M_GhostTrail_Inst'");
	
	Material = UMaterialInstanceDynamic::Create(material, this);
	Material->SetVectorParameterValue("Color", Color);
	Material->SetScalarParameterValue("Exp", Exponent);
	//

	//Owner설정
	Owner = Cast<ACharacter>(GetOwner());

	//Mesh 세팅
	Mesh->SetVisibility(false);
	Mesh->SetSkeletalMesh(Owner->GetMesh()->SkeletalMesh);
	Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());	//시작할때 캡쳐를 한번 해놔야 잘 먹힘 (원본본을 하나 복사해두는것)
	Mesh->SetRelativeScale3D(Scale);


	for (int32 i = 0; i < Owner->GetMesh()->SkeletalMesh->Materials.Num(); i++)
		Mesh->SetMaterial(i, Material);

	//Timer 세팅
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
	{
		if (Mesh->IsVisible() == false)
			Mesh->ToggleVisibility();

		//방향 맞춰주기
		float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		SetActorLocation(Owner->GetActorLocation() - FVector(ScaleAmount.X, ScaleAmount.Y, height - ScaleAmount.Z));
		SetActorRotation(Owner->GetActorRotation() + FRotator(0, -90, 0));

		//일정시간마다 캡쳐
		Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());	

	});

	//Timer 동작
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, timerDelegate, Interval, true, StartDelay);


}

void ACGhostTrail::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//타이머 제거
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

}

void ACGhostTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

