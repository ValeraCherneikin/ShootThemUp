// Shoot Them Up Game.All Rights Reserved.


#include "Player/STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase,All,All);

void ASTUPlayerState::LogInfo()
{
    UE_LOG(LogSTUGameModeBase,Display,TEXT("TeamID: %i, kills: %i, Death: %i"),TeamID,killsNum,DeathsNum);
}