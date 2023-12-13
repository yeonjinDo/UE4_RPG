#include "Components/CAIBehaviorComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"



UCAIBehaviorComponent::UCAIBehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCAIBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCAIBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

//bool Is(State)Mode() Definition

	bool UCAIBehaviorComponent::IsWaitMode()
	{
		return GetType() == EAIStateType::Wait;
	}

	bool UCAIBehaviorComponent::IsApproachMode()
	{
		return GetType() == EAIStateType::Approach;
	}

	bool UCAIBehaviorComponent::IsActionMode()
	{
		return GetType() == EAIStateType::Action;
	}


bool UCAIBehaviorComponent::IsPatrolMode()
	{
		return GetType() == EAIStateType::Patrol;
	}

	bool UCAIBehaviorComponent::IsHittedMode()
	{
		return GetType() == EAIStateType::Hitted;
	}

	bool UCAIBehaviorComponent::IsParryingMode()
	{
		return GetType() == EAIStateType::Parrying;
	}

	bool UCAIBehaviorComponent::IsAvoidMode()
	{
		return GetType() == EAIStateType::Avoid;
	}

	bool UCAIBehaviorComponent::IsDeadMode()
	{
		return GetType() == EAIStateType::Dead;
	}

bool UCAIBehaviorComponent::IsIdle()
{
	return GetActionType() == EBossActionType::Idle;
}

bool UCAIBehaviorComponent::IsPhaseOne()
{
	return GetActionType() == EBossActionType::PhaseOne;
}

bool UCAIBehaviorComponent::IsHitted()
{
	return GetActionType() == EBossActionType::Hitted;
}

bool UCAIBehaviorComponent::IsPhaseThree()
{
	return GetActionType() == EBossActionType::PhaseThree;
}

bool UCAIBehaviorComponent::IsEndAction()
{
	return GetActionType() == EBossActionType::EndAction;
}

//

//void Set(State)Mode() Definition

	void UCAIBehaviorComponent::SetWaitMode()
	{
		ChangeType(EAIStateType::Wait);
	}

	void UCAIBehaviorComponent::SetApproachMode()
	{
		ChangeType(EAIStateType::Approach);
	}

	void UCAIBehaviorComponent::SetActionMode()
	{
		ChangeType(EAIStateType::Action);
	}

	void UCAIBehaviorComponent::SetPatrolMode()
	{
		ChangeType(EAIStateType::Patrol);
	}

	void UCAIBehaviorComponent::SetHittedMode()
	{
		ChangeType(EAIStateType::Hitted);
	}

	void UCAIBehaviorComponent::SetParryingMode()
	{
		ChangeType(EAIStateType::Parrying);
	}

	void UCAIBehaviorComponent::SetAvoidMode()
	{
		ChangeType(EAIStateType::Avoid);
	}

	void UCAIBehaviorComponent::SetDeadMode()
	{
		ChangeType(EAIStateType::Dead);
	}

void UCAIBehaviorComponent::SetIdle()
{
	ChangePhase(EBossActionType::Idle);
}


void UCAIBehaviorComponent::SetPhaseOne()
	{
		ChangePhase(EBossActionType::PhaseOne);
	}

	void UCAIBehaviorComponent::SetHitted()
	{
		ChangePhase(EBossActionType::Hitted);
	}

	void UCAIBehaviorComponent::SetPhaseThree()
	{
		ChangePhase(EBossActionType::PhaseThree);
	}

	void UCAIBehaviorComponent::SetEndAction()
	{
		ChangePhase(EBossActionType::EndAction);
	}

//

FVector UCAIBehaviorComponent::GetPatrolLocation()
{
	return Blackboard->GetValueAsVector(PatrolLocationKey);
}

void UCAIBehaviorComponent::SetPatrolLocation(const FVector& InLocation)
{
	Blackboard->SetValueAsVector(PatrolLocationKey, InLocation);
}

FVector UCAIBehaviorComponent::GetAvoidLocation()
{
	return Blackboard->GetValueAsVector(AvoidLocationKey);
}

TArray<FVector> UCAIBehaviorComponent::GetRandomPoints()
{
	return RandomLocations;
}


void UCAIBehaviorComponent::SetRandomPoints(const TArray<FVector>& InLocations)
{
	RandomLocations = InLocations;
}

FTransform UCAIBehaviorComponent::GetNotifyTransform()
{
	return NotifyTransform;
}

void UCAIBehaviorComponent::SetNotifyTransform(const FTransform& InTransform)
{
	NotifyTransform = InTransform;
}


EAIStateType UCAIBehaviorComponent::GetType()
{
	return (EAIStateType)Blackboard->GetValueAsEnum(AIStateTypeKey);
}

EBossActionType UCAIBehaviorComponent::GetActionType()
{
	return (EBossActionType)Blackboard->GetValueAsEnum(BossPhaseKey);
}

ACharacter* UCAIBehaviorComponent::GetTarget()
{
	return Cast<ACharacter>(Blackboard->GetValueAsObject(TargetKey));
}

void UCAIBehaviorComponent::ChangeType(EAIStateType InType)
{
	EAIStateType prevType = GetType();

	Blackboard->SetValueAsEnum(AIStateTypeKey, (uint8)InType);

	if (OnAIStateTypeChanged.IsBound())
		OnAIStateTypeChanged.Broadcast(prevType, InType);
}

void UCAIBehaviorComponent::ChangePhase(EBossActionType InType)
{
	EBossActionType prevType = GetActionType();

	Blackboard->SetValueAsEnum(BossPhaseKey, (uint8)InType);

}
