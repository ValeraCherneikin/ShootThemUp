// Shoot Them Up Game.All Rights Reserved.


#include "Pickups/STUHealthPickup.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup,All,All);

bool ASTUHealthPickup::GivePickupTrue(APawn* PlayerPawn)
{

    UE_LOG(LogHealthPickup, Display, TEXT("Health was taken"));
    return true;
}
