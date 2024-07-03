// Shoot Them Up Game.All Rights Reserved.


#include "UI/STUSpectatorWidget.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"

bool USTUSpectatorWidget::GetRespawnTime(int32& CountDownTimer) const
{
    const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(GetOwningPlayer());
    if(!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) return false;

    CountDownTimer = RespawnComponent->GetRespawnCountDown();
    return true;
}