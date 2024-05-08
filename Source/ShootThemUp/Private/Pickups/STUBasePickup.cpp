// Shoot Them Up Game.All Rights Reserved.


#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup,All,All);

ASTUBasePickup::ASTUBasePickup()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponents");
    CollisionComponent -> InitSphereRadius(50.0f);
    CollisionComponent ->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent -> SetCollisionResponseToAllChannels(ECR_Overlap);
    SetRootComponent(CollisionComponent);

}


void ASTUBasePickup::BeginPlay()
{
	Super::BeginPlay();

    check(CollisionComponent);
	
}


void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    const  auto Pawn = Cast<APawn>(OtherActor);
    if(GivePickupTrue(Pawn))
    {
        PickupWasTaken();
    }
}


void ASTUBasePickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

bool ASTUBasePickup::GivePickupTrue(APawn* PlayerPawn)
{
    return false;
}

void ASTUBasePickup::PickupWasTaken()
{
    CollisionComponent -> SetCollisionResponseToAllChannels(ECR_Ignore);
    if(GetRootComponent())
    {
        GetRootComponent()->SetVisibility(false, true);
    }
    FTimerHandle RespawnTimerHandle;
    GetWorldTimerManager().SetTimer(RespawnTimerHandle,this,&ASTUBasePickup::Respawn,RespawnTime);
}

void ASTUBasePickup::Respawn()
{
    CollisionComponent -> SetCollisionResponseToAllChannels(ECR_Overlap);
    if(GetRootComponent())
    {
        GetRootComponent()->SetVisibility(true, true);
    }
}

