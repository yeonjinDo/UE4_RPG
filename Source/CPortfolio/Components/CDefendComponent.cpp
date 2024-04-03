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
{	//Defend ����, tab�����µ��� ��Ÿ�� ���� �ݺ�

	Montages->PlayDefendMode();	
}

void UCDefendComponent::Parrying()
{
	Montages->PlayParryingMode();
	//CLog::Log("Parrying");
}

void UCDefendComponent::EndDefend()
{	//Defend ��, tab ������ ��Ÿ�� ���� ���� ����

	if(State->IsDefendMode())	//defend�� parrying ���� �� parrying��� ������ ����ǵ��� �ϱ����� ����
		Montages->PlayEndDefendMode();
}

void UCDefendComponent::End_Defend()
{	//Defend �� ���� ����
	IICharacter::End_Defend();

	State->SetIdleMode();
}

