// Shoot Them Up Game.All Rights Reserved.


#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


ASTUProjectile::ASTUProjectile()
{
 	
	PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponents");
    CollisionComponent -> InitSphereRadius(5.0f);
    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectMovementComponent"));
    MovementComponent ->InitialSpeed = 2000.0f;
    MovementComponent->ProjectileGravityScale = 0.0f;

}


void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();
    check(MovementComponent);
    MovementComponent->Velocity = ShootDirection * MovementComponent->InitialSpeed;
    SetLifeSpan(5.0f);
}


