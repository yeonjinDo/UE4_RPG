#include "Components/CMontagesComponent.h"

#include "CMovementComponent.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "GameFramework/Character.h"
#include "Weapons/CWeaponStructures.h"

//#define LOG_UCMontagesComponent 1

UCMontagesComponent::UCMontagesComponent()
{

}

void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

	//DT_Player / DT_Enemy 데이터 불러오기
	if(DataTable == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "DataTable InValid");
		return;
	}

	OwnerCharacter = Cast<ACharacter>(GetOwner());


	TArray<FMontagesData*> datas;
	DataTable->GetAllRows<FMontagesData>("",	 datas);

	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		for (FMontagesData* data : datas)
		{
			if((EStateType)i == data->Type)
			{
				Datas[i] = data;

				continue;
			}
		}
	}


	//DT_AvoidAttack 데이터 불러오기
	if (AvoidAttackDataTable == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "AvoidAttackDataTable InValid");
		return;
	}


	TArray<FAvoidAttackData*> avoidAttackDatas;
	AvoidAttackDataTable->GetAllRows<FAvoidAttackData>("", avoidAttackDatas);

	for (int32 i = 0; i < (int32)EWeaponType::Max; i++)
	{
		for (FAvoidAttackData* data : avoidAttackDatas)
		{
			if ((EWeaponType)i == data->Type)
			{
				AvoidAttackDatas[i] = data;

				continue;
			}
		}
	}



#if LOG_UCMontagesComponent
	for (FMontagesData* data : datas)
	{
		if (data == nullptr)
			continue;

		FString str;
		str.Append(StaticEnum<EStateType>()->GetValueAsString(data->Type));
		str.Append(" / ");
		str.Append(data->Montage->GetPathName());

		CLog::Log(str);
	}
#endif

}

void UCMontagesComponent::PlayBackStepMode()
{
	PlayAnimMontage(EStateType::BackStep);
}

void UCMontagesComponent::PlayRightRollMode()
{
	PlayAnimMontage(EStateType::RightRoll);
}

void UCMontagesComponent::PlayLeftRollMode()
{
	PlayAnimMontage(EStateType::LeftRoll);
}

void UCMontagesComponent::PlayAvoidMode()
{
	PlayAnimMontage(EStateType::Avoid);
}

void UCMontagesComponent::PlayCounterMode()
{
	UCWeaponComponent*   weapon   = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);

	PlayAnimMontage(weapon->GetWeaponType());
	
}

void UCMontagesComponent::PlayDefendMode()
{
	PlayAnimMontage(EStateType::Defend);
}

void UCMontagesComponent::PlayEndDefendMode()
{
	PlayAnimMontage(EStateType::Defend, "loopend");
}

void UCMontagesComponent::PlayParryingMode()
{
	PlayAnimMontage(EStateType::Parrying);
}

void UCMontagesComponent::PlayDeadMode()
{
	PlayAnimMontage(EStateType::Dead);
}

void UCMontagesComponent::PlayRiseMode()
{
	PlayAnimMontage(EStateType::Rise);
}

void UCMontagesComponent::PlayExecuteMode()
{
	PlayAnimMontage(EStateType::Execute);
}

void UCMontagesComponent::PlayJumpMode()
{
	PlayAnimMontage(EStateType::Jump);
}

void UCMontagesComponent::PlayDoubleJumpMode()
{
	PlayAnimMontage(EStateType::DoubleJump);
}

void UCMontagesComponent::PlayHittedMode()
{
	PlayAnimMontage(EStateType::Hitted);
}

void UCMontagesComponent::PlayTurnLeftMode()
{
	PlayAnimMontage(EStateType::TurnLeft);
}

void UCMontagesComponent::PlayTurnRightMode()
{
	PlayAnimMontage(EStateType::TurnRight);
}

void UCMontagesComponent::PlayAnimMontage(EWeaponType InType)
{
	//AvoidAttack

	CheckNull(OwnerCharacter);

	FAvoidAttackData* data = AvoidAttackDatas[(int32)InType];
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);

	if (data == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "No Data");

		return;
	}

	if (data->Montage == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "No Montage Data");

		return;
	}

	if(!!data->Montage)
	{
		OwnerCharacter->SetActorLocation(OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * 350);
		OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);

		movement->EnableFixedCamera();
	}
		
}

void UCMontagesComponent::PlayAnimMontage(EStateType InType)
{
	CheckNull(OwnerCharacter);

	FMontagesData* data = Datas[(int32)InType];

	if(data == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "No Montage Data");

		return;
	}

	if (data->Montage == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "No Montage Data");

		return;
	}

	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);

}

void UCMontagesComponent::PlayAnimMontage(EStateType InType, FName InSectionName)
{
	CheckNull(OwnerCharacter);

	FMontagesData* data = Datas[(int32)InType];

	if (data == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "No Montage Data");

		return;
	}

	if (data->Montage == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "No Montage Data");

		return;
	}

	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate, InSectionName);
}

