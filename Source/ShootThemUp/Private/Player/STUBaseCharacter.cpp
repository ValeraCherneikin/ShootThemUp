// Shoot Them Up Game.All Rights Reserved.


#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/DamageEvents.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter,All,All)
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f,100.0f,80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>(TEXT("HealthComponent"));

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthTextComponent"));
    HealthTextComponent -> SetupAttachment(GetRootComponent());
    HealthTextComponent -> SetOwnerNoSee(true);

    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>(TEXT("WeaponComponent"));
}

void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());
    
    OnHealthChanged(HealthComponent->GetHealth());
    HealthComponent -> OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
    HealthComponent -> FOnHealthChange.AddUObject(this,&ASTUBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this,&ASTUBaseCharacter::OnGroundLanded);
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);
    check(WeaponComponent);

    PlayerInputComponent->BindAxis("MoveForward",this,&ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight",this,&ASTUBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp",this,&ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround",this,&ASTUBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ASTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run",IE_Pressed,this,&ASTUBaseCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run",IE_Released,this,&ASTUBaseCharacter::OnStopRunning);
    PlayerInputComponent->BindAction("Fire",IE_Pressed,WeaponComponent,&USTUWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire",IE_Released,WeaponComponent,&USTUWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon",IE_Pressed,WeaponComponent,&USTUWeaponComponent::NextWeapon);
}

bool ASTUBaseCharacter::bIsRunning() const
{
    return bWantsToRun && bIsMovingForward && !GetVelocity().IsZero();
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    if(GetVelocity().IsZero()) return 0.0f;
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath :: Acos(FVector:: DotProduct(GetActorForwardVector(),VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(),VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees: Degrees * FMath::Sign(CrossProduct.Z);
    
}

void ASTUBaseCharacter::MoveForward(float Amount)
{
    bIsMovingForward = Amount > 0.0f;
    if(Amount == 0.0f) return;
    AddMovementInput(GetActorForwardVector(),Amount);
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
    if(Amount == 0.0f) return;
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

void ASTUBaseCharacter::OnDeath()
{
    UE_LOG(LogBaseCharacter,Display, TEXT("player %s is dead"),*GetName());

    PlayAnimMontage(DeathAnimMontage);

    GetCharacterMovement() -> DisableMovement();

    SetLifeSpan(5.0f);
    if(Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
    GetCapsuleComponent()-> SetCollisionResponseToAllChannels(ECR_Ignore);
    WeaponComponent->StopFire(); 
}

void ASTUBaseCharacter::OnHealthChanged(float Health)
{
    HealthTextComponent -> SetText(FText::FromString(FString::Printf(TEXT("%.0f"),Health)));
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto FallVelocityZ = -GetCharacterMovement()->Velocity.Z;
    UE_LOG(LogBaseCharacter,Display,TEXT("On landed %f"),FallVelocityZ);

    if(FallVelocityZ < LandedDamageVelocity.X) return;

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity,LandedDamage,FallVelocityZ);
    UE_LOG(LogBaseCharacter,Display,TEXT("FinalDamage %f"),FinalDamage);
    TakeDamage(FinalDamage,FDamageEvent{},nullptr,nullptr);
}




