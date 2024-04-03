#include "Characters/CEnemy_A_Nerd.h"
#include "Widgets/CUserWidget_Label.h"

void ACEnemy_A_Nerd::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR

	FString name = TEXT("갑옷 해골");
	label->UpdateName(FText::FromString(name));

#endif
}

FGenericTeamId ACEnemy_A_Nerd::GetGenericTeamId() const
{
	return FGenericTeamId(3);
}



