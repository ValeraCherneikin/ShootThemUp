// Shoot Them Up Game.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChange,float)



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUHealthComponent();

    float GetHealth() const {return Health;}

    UFUNCTION(BlueprintCallable)
    bool IsDead() const {return FMath::IsNearlyZero(Health);}

    FOnDeath OnDeath;
    FOnHealthChange FOnHealthChange;
    

    

protected:
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Health", meta = (ClampMin = 0.0f,ClampMax = 1000.0f))
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Heal")
    bool AutoHeal = true;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Heal",meta = (EditCondition= "AutoHeal"))
    float HealUpdateTime = 1.0f;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Heal",meta = (EditCondition= "AutoHeal"))
    float HealDelay = 3.0f;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Heal",meta = (EditCondition= "AutoHeal"))
    float HealModifire = 5.0f;
    
    
	virtual void BeginPlay() override;
private:
    float Health = 0.0f;
    FTimerHandle HealTimerHandl;
    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser );

    void HealUpdate();
    void SetHealth(float NewHealth);
};
