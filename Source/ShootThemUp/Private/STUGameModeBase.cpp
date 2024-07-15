// Shoot Them Up Game.All Rights Reserved.


#include "STUGameModeBase.h"
#include "AIController.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "Player/STUPlayerState.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"
#include "EngineUtils.h"

// DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase,All,All);

constexpr static int MinRoundTimeForRespawn = 10;
ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass      = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass              = ASTUGameHUD::StaticClass();
    PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();
    CreateTeamsInfo();
    
    CurrentRound = 1;
    StartRound();

    SetMatchState(ESTUMatchState::InProgress);
    
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASTUGameModeBase::killed(AController* KillerController, AController* VictimController)
{
    const auto killerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
    const auto VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;

    if(killerPlayerState)
    {
        killerPlayerState->addKill();
    }

    if(VictimPlayerState)
    {
        VictimPlayerState->addDeath();
    }

    StartRespawn(VictimController);
}

void ASTUGameModeBase::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
}

bool ASTUGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const auto PauseSet = Super::SetPause(PC,CanUnpauseDelegate);
    
    if(PauseSet)
    {
        SetMatchState(ESTUMatchState::Pause);
    }
    return PauseSet;
    
}

bool ASTUGameModeBase::ClearPause()
{
    const auto PauseCleared = Super::ClearPause();
    if(PauseCleared)
    {
        SetMatchState(ESTUMatchState::InProgress);
    }
    return PauseCleared;
}

void ASTUGameModeBase::SpawnBots()
{
    if (!GetWorld()) return;;

    for (int32 i = 0; i < GameData.PlayerNum - 1; ++i)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        RestartPlayer(STUAIController);
    }
}

void ASTUGameModeBase::StartRound()
{
   RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandl,this,&ASTUGameModeBase::GameTimerUpdate,1.0f,true);
}

void ASTUGameModeBase::GameTimerUpdate()
{
    //UE_LOG(LogSTUGameModeBase,Display,TEXT("item: %i / Round: i%/%i"), RoundCountDown, CurrentRound,GameData.RoundsNum);

    if(--RoundCountDown == 0)
    {
       GetWorldTimerManager().ClearTimer(GameRoundTimerHandl);

        if(CurrentRound + 1 <= GameData.RoundsNum)
        {
            ++CurrentRound;
            ResetPlayers();
            StartRound();
        }
        else
        {
            GameOver();
            //UE_LOG(LogSTUGameModeBase,Display,TEXT("------ GAME OVER ------"));
            LogPlayerInfo();
        }
    }
}

void ASTUGameModeBase::ResetPlayers()
{
   if(!GetWorld()) return;

    for(auto It = GetWorld()->GetControllerIterator();It;++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
    if(!Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void ASTUGameModeBase::CreateTeamsInfo()
{
    int32 TeamID = 1;
    if(!GetWorld()) return;
    for(auto It = GetWorld()->GetControllerIterator();It;++It)
    {
        const auto Controller = It->Get();
        if(!Controller) continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if(!PlayerState) continue;

        PlayerState->SetTeamID(TeamID);
        PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
        PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
        SetPlayerColor(Controller);
        
        TeamID = TeamID == 1 ? 2 : 1;
    }
}

FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
    if(TeamID - 1<GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamID - 1 ];
    }

    //UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for team id: %i, set to default: %s"), TeamID,*GameData.DefaultTeamColor.ToString());
    return GameData.DefaultTeamColor;
}

void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
    if(!Controller) return;

    const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
    if(!Character) return;

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    if(!PlayerState) return;

    Character->SetPlayerColor(PlayerState->GetTeamColor());
    
}

void ASTUGameModeBase::LogPlayerInfo()
{
    if(!GetWorld()) return;
    
    for(auto It = GetWorld()->GetControllerIterator();It;++It)
    {
        const auto Controller = It->Get();
        if(!Controller) continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if(!PlayerState) continue;

        PlayerState->LogInfo();
    }
}

void ASTUGameModeBase::StartRespawn(AController* Controller)
{
    const auto RespawnAvailable = RoundCountDown > MinRoundTimeForRespawn  + GameData.RespawnTime;
    if(!RespawnAvailable) return;
    const auto RespawnComponents = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(Controller);
    if(!RespawnComponents) return;

    RespawnComponents->Respawn(GameData.RespawnTime);
}

void ASTUGameModeBase::GameOver()
{
    LogPlayerInfo();

    for(auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if(Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }

    SetMatchState(ESTUMatchState::GameOver);
}

void ASTUGameModeBase::SetMatchState(ESTUMatchState State)
{
    if(MatchState == State)return;

    MatchState = State;
    OnMatchStateChanged.Broadcast(MatchState);
}