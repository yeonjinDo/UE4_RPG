#include "Components/CSubStateComponent.h"

UCSubStateComponent::UCSubStateComponent()
{
}


void UCSubStateComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UCSubStateComponent::SetGroundMode()
{
	ChangeType(ESubStateType::Ground);
}

void UCSubStateComponent::SetDashMode()
{
	ChangeType(ESubStateType::Dash);
}

void UCSubStateComponent::SetFlyMode()
{
	ChangeType(ESubStateType::Fly);
}

void UCSubStateComponent::SetAirComboMode()
{
	ChangeType(ESubStateType::AirCombo);
}

void UCSubStateComponent::SetFallMode()
{
	ChangeType(ESubStateType::Fall);
}

void UCSubStateComponent::SetFaintMode()
{
	ChangeType(ESubStateType::Faint);
}

void UCSubStateComponent::ChangeType(ESubStateType InType)
{
	ESubStateType prevType = Type;
	Type = InType;

	if (OnSubStateTypeChanged.IsBound())
		OnSubStateTypeChanged.Broadcast(prevType, Type);
}


