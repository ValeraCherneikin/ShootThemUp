// Shoot Them Up Game.All Rights Reserved.


#include "Player/STUPlayerController.h"
#include "Player/STUBaseCharacter.h"
#include "Components/STURespawnComponent.h"


ASTUPlayerController::ASTUPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>(TEXT("RespawnComponent"));
}