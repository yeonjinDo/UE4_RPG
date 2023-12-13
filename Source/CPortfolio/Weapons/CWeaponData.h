#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponData.generated.h"

UCLASS()
class CPORTFOLIO_API UCWeaponData : public UObject
{
	GENERATED_BODY()


private:	//UCWeaponAsset에서 접근하는것
	friend class UCWeaponAsset;


public:
	FORCEINLINE class ACAttachment*   GetAttachment()    { return Attachment; }
	FORCEINLINE class ACAttachment*   GetSubAttachment() { return SubAttachment; }
	FORCEINLINE class UCEquipment*    GetEquipment()     { return Equipment; }
	FORCEINLINE class UCDoAction*     GetDoAction()      { return DoAction; }
	FORCEINLINE class UCSubAction*    GetSubAction()     { return SubAction; }
	FORCEINLINE class UCSkillAction*  GetSkillAction()   { return SkillAction; }


private:
	UPROPERTY()
		class ACAttachment* Attachment;

	UPROPERTY()
		class ACAttachment* SubAttachment;

	UPROPERTY()
		class UCEquipment*  Equipment;

	UPROPERTY()
		class UCDoAction*   DoAction;

	UPROPERTY()
		class UCSubAction*  SubAction;

	UPROPERTY()
		class UCSkillAction*  SkillAction;
};
