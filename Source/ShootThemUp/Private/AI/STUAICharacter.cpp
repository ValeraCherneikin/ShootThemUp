// Shoot Them Up Game.All Rights Reserved.


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit): Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ASTUAIController::StaticClass();

    bUseControllerRotationYaw = false;
    
    if(CharacterMovementComponent)
    {
        CharacterMovementComponent->bUseControllerDesiredRotation = true;
        CharacterMovementComponent->RotationRate = FRotator(0.0f,200.0f,0.0f);
    }
}

void ASTUAICharacter::OnDeath()
{
    Super::OnDeath();

    const auto STUController = Cast<AAIController>(Controller);

    if(STUController && STUController->BrainComponent)
    {
        STUController->BrainComponent->Cleanup();
    }
}