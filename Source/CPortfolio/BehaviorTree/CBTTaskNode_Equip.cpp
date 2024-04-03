#include "BehaviorTree/CBTTaskNode_Equip.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Characters/CAIController_Boss.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Weapons/CEquipment.h"

UCBTTaskNode_Equip::UCBTTaskNode_Equip()
{
	NodeName = "Equip";

	bNotifyTick = true;

}

EBTNodeResult::Type UCBTTaskNode_Equip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	if (Type == weapon->GetWeaponType())
		return EBTNodeResult::Succeeded;

	switch (Type)
	{
		case EWeaponType::Fist:
			weapon->SetFistMode();
			break;

		case EWeaponType::Sword: 
			weapon->SetSwordMode();
			break;

		case EWeaponType::Hammer:
			weapon->SetHammerMode();
			break;

		case EWeaponType::Warp:
			weapon->SetWarpMode();
			break;

		case EWeaponType::Around:
			weapon->SetAroundMode();
			break;

		case EWeaponType::Bow:
			weapon->SetBowMode();
			break;

	}

	return EBTNodeResult::InProgress;

}

void UCBTTaskNode_Equip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCStateComponent*  state  = CHelpers::GetComponent<UCStateComponent>(ai);

	
	//장착중이고 idle모드일때만
	const bool* bEquipped = weapon->GetEquipment()->GetEquipped();
	if(state->IsIdleMode() && *bEquipped)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		return;
	}
}

EBTNodeResult::Type UCBTTaskNode_Equip::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	CheckTrueResult(aiState->IsDeadMode(), EBTNodeResult::Failed );

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);

	if (weapon == nullptr)
		return EBTNodeResult::Failed;

	bool bBeginEquip = weapon->GetEquipment()->GetBeginEquipped();

	if (bBeginEquip == false)
		weapon->GetEquipment()->Begin_Equip();
	
	weapon->GetEquipment()->End_Equip();


	//중단으로 끝내서 루트로 보낸다 Succeeded로 하면 바로 옆 노드로 이동 
	return EBTNodeResult::Aborted;


}
