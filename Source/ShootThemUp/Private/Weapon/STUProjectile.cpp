// Shoot Them Up Game.All Rights Reserved.


#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Components/STUWeaponFXComponent.h"

ASTUProjectile::ASTUProjectile()
{
 	
	PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponents");
    CollisionComponent -> InitSphereRadius(5.0f);
    CollisionComponent ->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent -> SetCollisionResponseToAllChannels(ECR_Block);
    CollisionComponent->bReturnMaterialOnMove = true;
    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectMovementComponent"));
    MovementComponent ->InitialSpeed = 2000.0f;
    MovementComponent->ProjectileGravityScale = 0.0f;

    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>(TEXT("WeaponFXComponent"));

}


void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();
    check(MovementComponent);
    check(CollisionComponent);
    check(WeaponFXComponent);

    MovementComponent->Velocity = ShootDirection * MovementComponent->InitialSpeed;
    CollisionComponent->IgnoreActorWhenMoving(GetOwner(),true);
    CollisionComponent -> OnComponentHit.AddDynamic(this,&ASTUProjectile::OnProjectileHit);
    SetLifeSpan(LifeSeconds);
}

void ASTUProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if(!GetWorld()) return;

    MovementComponent->StopMovementImmediately();

    //make damage
    UGameplayStatics::ApplyRadialDamage(GetWorld(), //
        DamageAmount,                              //
        GetActorLocation(),                       //
        DamageRadius,                            //
        UDamageType::StaticClass(),             //
        {GetOwner()},                        //
        this,                                 //
        nullptr,                             //
        bDoFullDamage);

    // DrawDebugSphere(GetWorld(),GetActorLocation(),DamageRadius,24,FColor::Red,false,5.0f);
    WeaponFXComponent->PlayImpactFX(Hit);
    Destroy();
}

AController* ASTUProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}


