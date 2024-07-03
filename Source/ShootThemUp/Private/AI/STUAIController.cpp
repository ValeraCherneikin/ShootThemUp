// Shoot Them Up Game.All Rights Reserved.


#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STURespawnComponent.h"

ASTUAIController::ASTUAIController()
{
    STUAIPerceptionComponet = CreateDefaultSubobject<USTUAIPerceptionComponent>(TEXT("STUAIPerceptionComponen"));
    SetPerceptionComponent(*STUAIPerceptionComponet);

    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>(TEXT("RespawnComponent"));
    
    
    bWantsPlayerState = true;
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto STUCharacter = Cast<ASTUAICharacter>(InPawn);
    if(STUCharacter)
    {

       RunBehaviorTree(STUCharacter->BehaviorTreeAssets);

    }
}

void ASTUAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    const auto AimActor = GetFocusOnActor();
    SetFocus(AimActor);
}

AActor* ASTUAIController::GetFocusOnActor() const
{
    if(!GetBlackboardComponent()) return nullptr;
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
