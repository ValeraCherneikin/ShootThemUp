// Shoot Them Up Game.All Rights Reserved.


#include "UI/STUPlayerStartRawWidget.h"
#include <Components/TextBlock.h>
#include "Components/Image.h"

void USTUPlayerStartRawWidget::SetPlayerTextBlock(const FText& Text)
{
    if(!PlayerNameTextBlock) return;
    PlayerNameTextBlock->SetText(Text);
}

void USTUPlayerStartRawWidget::SetkillsTextBlock(const FText& Text)
{
    if(!killsTextBlock) return;
    killsTextBlock->SetText(Text);
}

void USTUPlayerStartRawWidget::SetDeathTextBlock(const FText& Text)
{
    if(!DeathTextBlock) return;
        DeathTextBlock->SetText(Text);
}

void USTUPlayerStartRawWidget::SetTeamTextBlock(const FText& Text)
{
    if(!TeamTextBlock) return;
    TeamTextBlock->SetText(Text);
}

void USTUPlayerStartRawWidget::SetPlayerIndicatorImage(bool Visible)
{
    if(!PlayerIndicatorImage) return;
    PlayerIndicatorImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}