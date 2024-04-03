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
		float Bend;		//활의 휘어짐 정도를 표현할 변수

public:
	FORCEINLINE float* GetBend() { return &Bend; }	//주소를 리턴받을 애, 외부에서 얘한테 값을 줄 것이다

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
};
