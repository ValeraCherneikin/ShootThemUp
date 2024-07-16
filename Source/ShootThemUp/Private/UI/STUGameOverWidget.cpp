// Shoot Them Up Game.All Rights Reserved.


#include "UI/STUGameOverWidget.h"
#include "STUGameModeBase.h"
#include "Player/STUPlayerState.h"
#include "UI/STUPlayerStartRawWidget.h"
#include "Components/VerticalBox.h"
#include "STUUtils.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
void USTUGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if(GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase> (GetWorld()->GetAuthGameMode());
        if(GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this,&USTUGameOverWidget::OnMatchStateChanged); 
        }
    }
    if(ResetLevelButton)
    {
        ResetLevelButton->OnClicked.AddDynamic(this,&USTUGameOverWidget::OnResetLevel);
    }
}

void USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState State)
{
    if(State == ESTUMatchState::GameOver)
    {
        UpdatePlayerStat();
    }
}

void USTUGameOverWidget::UpdatePlayerStat()
{
    if(!GetWorld() || !PlayerStatBox) return;

    PlayerStatBox->ClearChildren();

    for(auto It = GetWorld()->GetControllerIterator();It;++It)
    {
        const auto Controller = It->Get();
        if(!Controller) continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if(!PlayerState) continue;

        const auto PlayerStatRowWidget = CreateWidget<USTUPlayerStartRawWidget>(GetWorld(),PlayerStatRowWidgetClass);
        if(!PlayerStatRowWidget) continue;

        PlayerStatRowWidget->SetPlayerTextBlock(FText::FromString(PlayerState->GetPlayerName()));
        PlayerStatRowWidget->SetkillsTextBlock(STUUtils::TextFromInt(PlayerState->GetKillsNum()));
        PlayerStatRowWidget->SetDeathTextBlock(STUUtils::TextFromInt(PlayerState->GetDeathNum()));
        PlayerStatRowWidget->SetTeamTextBlock(STUUtils::TextFromInt(PlayerState->GetTeamID()));
        PlayerStatRowWidget->SetPlayerIndicatorImage(Controller->IsPlayerController());

        PlayerStatBox->AddChild(PlayerStatRowWidget);
    }
}

void USTUGameOverWidget::OnResetLevel()
{
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this,FName(CurrentLevelName));
}