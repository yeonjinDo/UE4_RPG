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

	//Dynamic Material �� �ֱ�
	UMaterialInstanceConstant* material;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Materials/M_GhostTrail_Inst.M_GhostTrail_Inst'");
	
	Material = UMaterialInstanceDynamic::Create(material, this);
	Material->SetVectorParameterValue("Color", Color);
	Material->SetScalarParameterValue("Exp", Exponent);
	//

	//Owner����
	Owner = Cast<ACharacter>(GetOwner());

	//Mesh ����
	Mesh->SetVisibility(false);
	Mesh->SetSkeletalMesh(Owner->GetMesh()->SkeletalMesh);
	Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());	//�����Ҷ� ĸ�ĸ� �ѹ� �س��� �� ���� (�������� �ϳ� �����صδ°�)
	Mesh->SetRelativeScale3D(Scale);


	for (int32 i = 0; i < Owner->GetMesh()->SkeletalMesh->Materials.Num(); i++)
		Mesh->SetMaterial(i, Material);

	//Timer ����
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
	{
		if (Mesh->IsVisible() == false)
			Mesh->ToggleVisibility();

		//���� �����ֱ�
		float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		SetActorLocation(Owner->GetActorLocation() - FVector(ScaleAmount.X, ScaleAmount.Y, height - ScaleAmount.Z));
		SetActorRotation(Owner->GetActorRotation() + FRotator(0, -90, 0));

		//�����ð����� ĸ��
		Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());	

	});

	//Timer ����
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, timerDelegate, Interval, true, StartDelay);


}

void ACGhostTrail::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//Ÿ�̸� ����
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

}

void ACGhostTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

