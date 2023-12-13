#include "Characters/CEnemy_AI_Range.h"
#include "Widgets/CUserWidget_Label.h"

void ACEnemy_AI_Range::BeginPlay()
{
	Super::BeginPlay();

	//TeamID = 2;
#if WITH_EDITOR

	FString name = TEXT("모자를 쓴 사나이");
	label->UpdateName(FText::FromString(name));

#endif
}

FGenericTeamId ACEnemy_AI_Range::GetGenericTeamId() const
{
	return FGenericTeamId(5);
}

