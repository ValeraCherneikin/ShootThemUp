// Shoot Them Up Game.All Rights Reserved.


#include "Player/STUPlayerController.h"
#include "Player/STUBaseCharacter.h"
#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"
#include "GameFramework/GameMode.h"


ASTUPlayerController::ASTUPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>(TEXT("RespawnComponent"));
}

void ASTUPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if(GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase> (GetWorld()->GetAuthGameMode());
        if(GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this,&ASTUPlayerController::OnMatchStateChange); 
        }
    } 
}

void ASTUPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    OnNewPawn.Broadcast(InPawn);
}

void ASTUPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if(!InputComponent) return;

    InputComponent->BindAction("PauseGame",IE_Pressed,this,&ASTUPlayerController::OnPauseGame);
}
void ASTUPlayerController::OnPauseGame()
{
    if(!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
    
    GetWorld()->GetAuthGameMode()->SetPause(this);
}

void ASTUPlayerController::OnMatchStateChange(ESTUMatchState State)
{
    if(State == ESTUMatchState::InProgress)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}