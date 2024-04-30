// Shoot Them Up Game.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUWeaponComponent();

    void StartFire();
    void StopFire();
    

protected:
    UPROPERTY(EditDefaultsOnly,Category = "Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;

    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly,Category = "Weapon")
    FName  WeaponAttachPointName = "WeaponSocket";
    

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;//??
    
    void SpawnWeapon();
		
};
