#include "CGameModeBase.h"
#include "Global.h"

ACGameModeBase::ACGameModeBase()
{
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Player/BP_CPlayer.BP_CPlayer_C'");

}
