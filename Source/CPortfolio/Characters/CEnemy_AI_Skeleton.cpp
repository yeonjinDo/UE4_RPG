#include "Characters/CEnemy_AI_Skeleton.h"
#include "Widgets/CUserWidget_Label.h"

ACEnemy_AI_Skeleton::ACEnemy_AI_Skeleton()
{
}

void ACEnemy_AI_Skeleton::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR

	FString name = TEXT("갑옷 병정");
	label->UpdateName(FText::FromString(name));

#endif
}

FGenericTeamId ACEnemy_AI_Skeleton::GetGenericTeamId() const
{
	return FGenericTeamId(4);
}
