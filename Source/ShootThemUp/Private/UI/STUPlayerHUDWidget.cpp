// Shoot Them Up Game.All Rights Reserved.


#include "UI/STUPlayerHUDWidget.h"

#include "STUWeaponComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"



float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = STUUtils :: GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    if(!HealthComponent) return 0.0f;

    return HealthComponent -> GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    
    const auto WeaponComponent = STUUtils :: GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if(!WeaponComponent) return false;

    return WeaponComponent -> GetCurrentWeaponUIData(UIData);
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    
    const auto WeaponComponent = STUUtils :: GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if(!WeaponComponent) return false;

    return WeaponComponent -> GetCurrentWeaponAmmoData(AmmoData);
}

bool USTUPlayerHUDWidget::isPlayerAlive() const
{
    const auto HealthComponent = STUUtils :: GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent &&  !HealthComponent -> IsDead();
}

bool USTUPlayerHUDWidget::isPlayerSpectating() const
{
    const auto Contoller = GetOwningPlayer();
    return Contoller && Contoller->GetStateName() == NAME_Spectating;
}

bool USTUPlayerHUDWidget::Initialize()
{
    const auto HealthComponent = STUUtils :: GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    if(HealthComponent)
    {
        HealthComponent->FOnHealthChange.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
    }
    return Super::Initialize();
}

void USTUPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
    if(HealthDelta<0.0f)
    {
        OnTakeDamage();   
    }
}

