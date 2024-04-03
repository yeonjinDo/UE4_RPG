#include "Characters/CEnemy_Strafe.h"
#include "Global.h"
#include "Widgets/CUserWidget_Label.h"

ACEnemy_Strafe::ACEnemy_Strafe()
{
}

void ACEnemy_Strafe::BeginPlay()
{
	 Super::BeginPlay();

#if WITH_EDITOR

	FString name = TEXT("길거리 도둑");
	label->UpdateName(FText::FromString(name));

#endif
}

FGenericTeamId ACEnemy_Strafe::GetGenericTeamId() const
{
	return FGenericTeamId(5);
}
