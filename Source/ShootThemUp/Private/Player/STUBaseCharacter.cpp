// Shoot Them Up Game.All Rights Reserved.


#include "Player/STUBaseCharacter.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/DamageEvents.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter,All,All)
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;
    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>(TEXT("HealthComponent"));
    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>(TEXT("WeaponComponent"));
}

void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

    check(HealthComponent);
    check(GetCharacterMovement());
    check(GetMesh());
    
    OnHealthChanged(HealthComponent->GetHealth(),0.0f);
    HealthComponent -> OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
    HealthComponent -> FOnHealthChange.AddUObject(this,&ASTUBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this,&ASTUBaseCharacter::OnGroundLanded);
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASTUBaseCharacter::bIsRunning() const
{
    return false;
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

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
     const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    if(!MaterialInst) return;

    MaterialInst->SetVectorParameterValue(MaterialColorName,Color);
}









void ASTUBaseCharacter::OnDeath()
{
    UE_LOG(LogBaseCharacter,Display, TEXT("player %s is dead"),*GetName());

    // PlayAnimMontage(DeathAnimMontage);

    GetCharacterMovement() -> DisableMovement();

    SetLifeSpan(5.0f);
    
    GetCapsuleComponent()-> SetCollisionResponseToAllChannels(ECR_Ignore);
    WeaponComponent->StopFire();

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
}

void ASTUBaseCharacter::OnHealthChanged(float Health,float HealthDelta)
{
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




