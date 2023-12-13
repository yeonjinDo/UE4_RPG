#include "Characters/CEnemy_AI_Skeleton.h"
#include "Widgets/CUserWidget_Label.h"

void ACEnemy_AI_Skeleton::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR

	FString name = TEXT("검은 해골");
	label->UpdateName(FText::FromString(name));

#endif
}

FGenericTeamId ACEnemy_AI_Skeleton::GetGenericTeamId() const
{
	return FGenericTeamId(4);
}
