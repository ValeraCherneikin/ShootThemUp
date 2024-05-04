// Shoot Them Up Game.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUHealthComponent();

    float GetHealth() const {return Health;}

    UFUNCTION(BlueprintCallable, Category="Health")
    bool IsDead() const {return FMath::IsNearlyZero(Health);}

    UFUNCTION(BlueprintCallable, Category="Health")
    float GetHealthPercent() const{return Health/MaxHealth;}
    

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
