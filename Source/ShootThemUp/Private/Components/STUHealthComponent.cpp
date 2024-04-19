// Shoot Them Up Game.All Rights Reserved.

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Dev/STUFireDamageType.h"
#include "Dev/STUIceDamageType.h"
DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent,All,All);

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

    Health = MaxHealth;
    FOnHealthChange.Broadcast(Health);

    AActor* ComponentOwner = GetOwner();
    if(ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this,&USTUHealthComponent::OnTakeAnyDamage);
    }
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser )
{
    if(Damage <= 0.0f || IsDead()) return;
    Health = FMath::Clamp(Health - Damage,0.0f,MaxHealth);
    FOnHealthChange.Broadcast(Health);

    if(IsDead())
    {
        OnDeath.Broadcast();
    }
}

