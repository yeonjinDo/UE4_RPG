#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CParkourComponent.generated.h"


UENUM(BlueprintType)
enum class EParkourArrowType : uint8
{	//화살표 종류
	Center = 0,
	Ceil,
	Floor,
	Left,
	Right,
	Land,
	Max,
};

UENUM(BlueprintType)
enum class EParkourType : uint8
{
	Climb = 0,
	Fall,
	Slide,
	Short,
	Normal,
	Wall,
	Max,
};

USTRUCT(BlueprintType)
struct FParkourData : public FTableRowBase
{
   GENERATED_BODY()

public:
   UPROPERTY(EditAnywhere)
      EParkourType Type;

   UPROPERTY(EditAnywhere)
      UAnimMontage* Montage;

   UPROPERTY(EditAnywhere)
      float PlayRatio = 1;

   UPROPERTY(EditAnywhere)
      FName SectionName;

   UPROPERTY(EditAnywhere)
      float MinDistance;

   UPROPERTY(EditAnywhere)
      float MaxDistance;

   UPROPERTY(EditAnywhere)
      float Extent;

   UPROPERTY(EditAnywhere)
      bool bFixedCamera;


public:
   void PlayMontage(class ACharacter* InCharacter);
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPORTFOLIO_API UCParkourComponent : public UActorComponent
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere, Category = "Data")
		UDataTable* DataTable;

private:
	UPROPERTY(EditAnywhere, Category = "Trace")
		float TraceDistance = 600;

	UPROPERTY(EditAnywhere, Category = "Trace")
		TEnumAsByte<EDrawDebugTrace::Type> DebugType;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float AvailableFrontAngle = 15;


private:
	UPROPERTY(EditAnywhere, Category = "Add-On")
		TSubclassOf<class ACGhostTrail> GhostTrailClass;

public:
	FORCEINLINE bool IsExecuting() { return Type != EParkourType::Max; } //파쿠르 수행중 이라는 뜻



public:	
	UCParkourComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void LineTrace(EParkourArrowType InType);	//파쿠르 장애물인지 체크하는 라인

	//파쿠르 장애물일 경우의 라인트레이스
	void CheckTrace_Center();
	void CheckTrace_Ceil();
    void CheckTrace_Floor();
    void CheckTrace_LeftRight();
	void CheckTrace_Land();

private: //파쿠르 수행 가능한건지 체크(모서리, 각도체크)
	bool Check_Obstacle();	


public:	//파쿠르 수행
	void DoParkour(bool bLanded = false);	//착지 했을 때 콜한건지 아닌지
	void End_DoParkour();

private: //Climb
	bool Check_ClimbMode();
	void DoParkour_Climb();
	void End_DoParkour_Climb();

public: //Fall
	bool Check_FallMode();
	void DoParkour_Fall();

private:  //Slide
	bool Check_SlideMode();
	void DoParkour_Slide();
	void End_DoParkour_Slide();

private:
	bool Check_ObstacleMode(EParkourType InType, FParkourData& OutData);
	void DoParkour_Obstacle(FParkourData& InData);
	void End_DoParkour_Obstacle();




private:
	TMap<EParkourType, TArray<FParkourData>> DataMap;

private:
	class ACharacter* OwnerCharacter;
	class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];


private:
	FHitResult HitResults[(int32)EParkourArrowType::Max];

private:
	AActor* HitObstacle;	//Hit된 애
	FVector HitObstacleExtent;	//Hit된 애의 부피

	float HitDistance;	//Hit된 거리
	float ToFrontYaw;	//Hit된 애의 정면각도


private:
	EParkourType Type = EParkourType::Max;

private:
	bool bFalling;

private:
	AActor* BackupObstacle;


private:
	class ACGhostTrail* GhostTrail;


};
