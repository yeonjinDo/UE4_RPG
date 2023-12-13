#include "Weapons/CDoAction_Defend.h"
#include "Global.h"

UCDoAction_Defend::UCDoAction_Defend()
{
}

void UCDoAction_Defend::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner,
	const TArray<FDoActionData>& InDoActionData, const TArray<FHitData>& InHitData)
{
	Super::BeginPlay(InAttachment, InEquipment, InOwner, InDoActionData, InHitData);

}

void UCDoAction_Defend::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
}

void UCDoAction_Defend::DoAction()
{
	Super::DoAction();
}

void UCDoAction_Defend::Begin_DoAction()
{
	Super::Begin_DoAction();
}

void UCDoAction_Defend::End_DoAction()
{
	Super::End_DoAction();

	
}
