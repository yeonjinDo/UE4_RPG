#include "Components/CStateComponent.h"
#include "Global.h"

UCStateComponent::UCStateComponent()
{

}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetBackStepMode()
{
	ChangeType(EStateType::BackStep);
}

void UCStateComponent::SetAvoidMode()
{
	ChangeType(EStateType::Avoid);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetDefendMode()
{
	ChangeType(EStateType::Defend);
}

void UCStateComponent::SetParryingMode()
{
	ChangeType(EStateType::Parrying);
}

void UCStateComponent::SetRiseMode()
{
	ChangeType(EStateType::Rise);
}

void UCStateComponent::SetExecuteMode()
{
	ChangeType(EStateType::Execute);
}

void UCStateComponent::SetJumpMode()
{
	ChangeType(EStateType::Jump);
}

void UCStateComponent::SetFrozeMode()
{
	ChangeType(EStateType::Froze);
}


void UCStateComponent::ChangeType(EStateType InType)
{
	EStateType prevType = Type;
	Type = InType;
	
	//CLog::Log(Type);
	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, Type);
}

void UCStateComponent::OnSubActionMode()
{
	bInSubActionMode = true;
}

void UCStateComponent::OffSubActionMode()
{
	bInSubActionMode = false;
}

