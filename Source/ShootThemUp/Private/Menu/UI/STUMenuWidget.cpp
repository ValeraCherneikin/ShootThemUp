// Shoot Them Up Game.All Rights Reserved.


#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
void USTUMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if(StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this,&USTUMenuWidget::OnStartGame);
    }
}

void USTUMenuWidget::OnStartGame()
{
    const FName StartTupLevelName = "TestLevel";
    UGameplayStatics::OpenLevel(this,StartTupLevelName); 
}