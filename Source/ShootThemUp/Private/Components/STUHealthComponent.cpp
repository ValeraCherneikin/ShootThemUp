// Shoot Them Up Game.All Rights Reserved.

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraShakeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent,All,All);

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
    if(IsDead() || IsHealthFull()) return false;
    SetHealth(Health + HealthAmount);
    return true;
}

bool USTUHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health ,MaxHealth);
}


void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

    check(MaxHealth>0);

    SetHealth(MaxHealth);

    AActor* ComponentOwner = GetOwner();
    if(ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this,&USTUHealthComponent::OnTakeAnyDamage);
    }
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser )
{
    if(Damage <= 0.0f || IsDead() || !GetWorld()) return;

    SetHealth(Health - Damage);
   
    if(IsDead())
    {
        OnDeath.Broadcast();
    }
    else if(AutoHeal)
    {
        GetWorld() -> GetTimerManager().SetTimer(HealTimerHandl,this,&USTUHealthComponent::HealUpdate,HealUpdateTime,true,HealDelay);
    }
    PlayCameraShake();
}

void USTUHealthComponent::HealUpdate()
{
   SetHealth(Health+HealModifire);

    if(IsHealthFull() && GetWorld())
    {
        GetWorld() -> GetTimerManager().ClearTimer(HealTimerHandl);
    }
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
    const auto NextHealth = FMath::Clamp(NewHealth,0.0f,MaxHealth);
    const auto HelthDelta = NextHealth - Health;
    Health = NextHealth;
    FOnHealthChange.Broadcast(Health,HelthDelta); 
}

void USTUHealthComponent::PlayCameraShake()
{
    if(IsDead()) return;
    const auto Player = Cast<APawn>(GetOwner());
    if(!Player) return;

    const auto Controller = Player -> GetController<APlayerController>();
    if(!Controller || !Controller->PlayerCameraManager) return;

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

