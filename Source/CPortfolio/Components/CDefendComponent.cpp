#include "Components/CDefendComponent.h"
#include "CStateComponent.h"
#include "CMontagesComponent.h"
#include "Global.h"
//#include "Characters/CEnemy.h"
#include "Characters/ICharacter.h"

UCDefendComponent::UCDefendComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCDefendComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	State    = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Montages = CHelpers::GetComponent<UCMontagesComponent>(OwnerCharacter);
}


void UCDefendComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}



void UCDefendComponent::OnDefend()
{
	CheckFalse(State->IsIdleMode());
	
	State->SetDefendMode();
}

void UCDefendComponent::Defend()
{	//Defend 시작, tab누르는동안 몽타주 섹션 반복

	Montages->PlayDefendMode();	
}

void UCDefendComponent::Parrying()
{
	Montages->PlayParryingMode();
	//CLog::Log("Parrying");
}

void UCDefendComponent::EndDefend()
{	//Defend 끝, tab 뗐을때 몽타주 종료 섹션 시작

	if(State->IsDefendMode())	//defend중 parrying 성공 시 parrying모션 끝까지 재생되도록 하기위한 조건
		Montages->PlayEndDefendMode();
}

void UCDefendComponent::End_Defend()
{	//Defend 후 상태 설정
	IICharacter::End_Defend();

	State->SetIdleMode();
}

