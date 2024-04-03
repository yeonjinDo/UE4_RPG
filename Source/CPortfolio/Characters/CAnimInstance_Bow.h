#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance_Bow.generated.h"

UCLASS()
class CPORTFOLIO_API UCAnimInstance_Bow : public UAnimInstance
{
	GENERATED_BODY()


protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Bend;		//Ȱ�� �־��� ������ ǥ���� ����

public:
	FORCEINLINE float* GetBend() { return &Bend; }	//�ּҸ� ���Ϲ��� ��, �ܺο��� ������ ���� �� ���̴�

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
};
