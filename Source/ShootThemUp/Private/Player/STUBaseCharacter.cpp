// Shoot Them Up Game.All Rights Reserved.


#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);

}

void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward",this,&ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight",this,&ASTUBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp",this,&ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround",this,&ASTUBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ASTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run",IE_Pressed,this,&ASTUBaseCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run",IE_Released,this,&ASTUBaseCharacter::OnStopRunning);
    
    

}

bool ASTUBaseCharacter::bIsRunning() const
{
    return bWantsToRun && bIsMovingForward && !GetVelocity().IsZero();
}

void ASTUBaseCharacter::MoveForward(float Amount)
{
    bIsMovingForward = Amount > 0.0f;
    AddMovementInput(GetActorForwardVector(),Amount);
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(),Amount);
}

void ASTUBaseCharacter::OnStartRunning()
{
    bWantsToRun = true; 
}

void ASTUBaseCharacter::OnStopRunning()
{
    bWantsToRun = false;
}


