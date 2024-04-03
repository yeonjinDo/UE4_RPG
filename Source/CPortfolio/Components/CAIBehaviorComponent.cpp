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

	bool UCAIBehaviorComponent::IsStrafeMode()
	{
		return GetType() == EAIStateType::Strafe;
	}



//void Set(State)Mode() Definition

	void UCAIBehaviorComponent::SetWaitMode()
	{
		ChangeType(EAIStateType::Wait);

	}

	void UCAIBehaviorComponent::SetApproachMode()
	{
		if (GetType() == EAIStateType::Action)
		{
			int cnt = Blackboard->GetValueAsInt("Attacking");
			Blackboard->SetValueAsInt("Attacking", --cnt);
		}

		ChangeType(EAIStateType::Approach);

	}

	void UCAIBehaviorComponent::SetActionMode()
	{
		if (GetType() != EAIStateType::Action)
		{
			int cnt = Blackboard->GetValueAsInt("Attacking");
			Blackboard->SetValueAsInt("Attacking", ++cnt);
		}

		ChangeType(EAIStateType::Action);
	}

	void UCAIBehaviorComponent::SetPatrolMode()
	{

		if(GetType() == EAIStateType::Action)
		{
			int cnt = Blackboard->GetValueAsInt("Attacking");
			Blackboard->SetValueAsInt("Attacking", --cnt);
		}
		
		ChangeType(EAIStateType::Patrol);

	}

	void UCAIBehaviorComponent::SetHittedMode()
	{
		if (GetType() == EAIStateType::Action ||GetType() == EAIStateType::Strafe)
		{
			int cnt = Blackboard->GetValueAsInt("Attacking");
			Blackboard->SetValueAsInt("Attacking", --cnt);
			CLog::Log("Hitted Cnt --");
		}

		ChangeType(EAIStateType::Hitted);

	}

	void UCAIBehaviorComponent::SetParryingMode()
	{
		if (GetType() == EAIStateType::Action)
		{
			int cnt = Blackboard->GetValueAsInt("Attacking");
			Blackboard->SetValueAsInt("Attacking", --cnt);
		}

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

	void UCAIBehaviorComponent::SetStrafeMode()
	{
		if (GetType() == EAIStateType::Action)
		{
			int cnt = Blackboard->GetValueAsInt("Attacking");
			Blackboard->SetValueAsInt("Attacking", --cnt);
		}

		ChangeType(EAIStateType::Strafe);
	}

//BOSS--->
bool UCAIBehaviorComponent::IsIdle()
{
	return GetActionType() == EBossActionType::Idle;
}

bool UCAIBehaviorComponent::IsReady()
{
	return GetActionType() == EBossActionType::Ready;
}

bool UCAIBehaviorComponent::IsAction()
{
	return GetActionType() == EBossActionType::Action;
}

bool UCAIBehaviorComponent::IsDash()
{
	return GetActionType() == EBossActionType::Dash;
}

//
void UCAIBehaviorComponent::SetIdle()
{
	ChangePhase(EBossActionType::Idle);
}

void UCAIBehaviorComponent::SetReady()
{
	ChangePhase(EBossActionType::Ready);
}

void UCAIBehaviorComponent::SetAction()
{
	ChangePhase(EBossActionType::Action);
}

void UCAIBehaviorComponent::SetDash()
{
	ChangePhase(EBossActionType::Dash);
}
//<---Boss//

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

	if (OnBossStateTypeChanged.IsBound())
		OnBossStateTypeChanged.Broadcast(prevType, InType);
}
