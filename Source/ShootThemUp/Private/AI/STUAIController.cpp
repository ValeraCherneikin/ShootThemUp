// Shoot Them Up Game.All Rights Reserved.


#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"

ASTUAIController::ASTUAIController()
{
    STUAIPerceptionComponet = CreateDefaultSubobject<USTUAIPerceptionComponent>(TEXT("STUAIPerceptionComponen"));
    SetPerceptionComponent(*STUAIPerceptionComponet);
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

    const auto AimActor = STUAIPerceptionComponet->GetClosesEnemy();
    SetFocus(AimActor);
}
