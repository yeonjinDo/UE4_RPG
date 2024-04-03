#include "BehaviorTree/CPatrolPath.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"


ACPatrolPath::ACPatrolPath()
{
	//드래그하는동안 호출되는것은 막겠다
	bRunConstructionScriptOnDrag = false;	

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USplineComponent>(this, &Spline, "Spline", Root);
	CHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Text", Root);

	//땅에 묻히면 보기 힘드니까 살짝 올려놓기
	Spline->SetRelativeLocation(FVector(0, 0, 30));
	Spline->bHiddenInGame = false;	//굳이 없어도 원래 안보임


	Text->SetRelativeLocation(FVector(0, 0, 120));
	Text->SetRelativeRotation(FRotator(0, 180, 0));
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	Text->TextRenderColor = FColor::Red;
	Text->bHiddenInGame = true;


}

void ACPatrolPath::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR						//Editor 전용 함수 -> if WITH_EDITOR 꼭 붙여줘야한다
	Text->Text = FText::FromString(GetActorLabel());
#endif

	Spline->SetClosedLoop(bLoop);
}

void ACPatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ACPatrolPath::GetMoveTo()
{
	return Spline->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
}

void ACPatrolPath::UpdateIndex()
{
	int32 count = Spline->GetNumberOfSplinePoints();

	if (bReverse)
	{
		if (Index > 0)
		{
			Index--;

			return;
		}

		if (Spline->IsClosedLoop())
		{
			Index = count - 1;

			return;
		}

		Index = 1;
		bReverse = false;

		return;
	}

	if (Index < count - 1)
	{
		Index++;

		return;
	}

	if (Spline->IsClosedLoop())
	{
		Index = 0;

		return;
	}

	Index = count - 2;
	bReverse = true;
}

