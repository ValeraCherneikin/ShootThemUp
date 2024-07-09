// Shoot Them Up Game.All Rights Reserved.


#include "UI/STUPauseWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"

bool USTUPauseWidget::Initialize()
{
    const auto InitStatus =  Super::Initialize();

    if(ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPaus);
    }

    return InitStatus;
}

void USTUPauseWidget::OnClearPaus()
{
    if(!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->ClearPause();
}