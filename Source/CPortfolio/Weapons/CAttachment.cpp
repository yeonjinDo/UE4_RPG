#include "Weapons/CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"


ACAttachment::ACAttachment()
{
	CHelpers::CreateComponent(this, &Root, "Root");
	
}

void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	//�������� OwnerCharacter�� �ִ� ���¿��� beginPlay�� �����ϱ� ������
	//OwnerCharacter�� ���� ����������Ѵ�.

	TArray<USceneComponent*> children;
	Root->GetChildrenComponents(true, children);
	//Root�� �پ��ִ� ������Ʈ���� �����ͼ� ShapeComponent�� ĳ������ �Ǵ��� Ȯ��
	//ĳ���� �ȴٸ� Box, Capsule, Sphere�� �ϳ��̴� collisions�� add

	for(USceneComponent* child : children)
	{
		UShapeComponent* shape = Cast<UShapeComponent>(child);

		if(!!shape)
		{
			shape->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic  (this, &ACAttachment::OnComponentEndOverlap);
			
			Collisions.Add(shape);
		}
	}

	OffCollisions();


	Super::BeginPlay();
}

void ACAttachment::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), InSocketName);
}

void ACAttachment::AttachToCollision(FName InSocketName)
{
	for(UShapeComponent* collision : Collisions)
	{
		if(collision->GetName() == InSocketName.ToString())
		{
			collision->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);

			return;
		}
	}
}

void ACAttachment::Throw()
{
	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
}

void ACAttachment::OnCollisions()
{	//NotifyState���� ȣ��

	if (OnAttachmentBeginCollision.IsBound())
		OnAttachmentBeginCollision.Broadcast();

	
	for(UShapeComponent* shape : Collisions)
	{
		shape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}


}

void ACAttachment::OffCollisions()
{
	if (OnAttachmentEndCollision.IsBound())
		OnAttachmentEndCollision.Broadcast();


	for (UShapeComponent* shape : Collisions)
	{
		shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
		Hitted.AddUnique(character);

	if (OnAttachmentBeginOverlap.IsBound())
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));


}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
		Hitted.Remove(character);

	if (OnAttachmentEndOverlap.IsBound())
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, Cast<ACharacter>(OtherActor));

}

