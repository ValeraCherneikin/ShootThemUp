// Shoot Them Up Game.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "Weapon")
    UAnimMontage* ReloadAnimMontage;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUWeaponComponent();

    void StartFire();
    void StopFire();
    void NextWeapon();
    void Reload();
    

protected:
    UPROPERTY(EditDefaultsOnly,Category = "Weapon")
    TArray<FWeaponData> WeaponData;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditDefaultsOnly,Category = "Weapon")
    FName  WeaponEquipSocketName = "WeaponSocket";

    UPROPERTY(EditDefaultsOnly,Category = "Weapon")
    FName  WeaponArmorySocketName = "ArmorySocket";

    UPROPERTY(EditDefaultsOnly,Category = "Animation")
    UAnimMontage* EquipAnimMontage;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;
    
    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    int32 CurrentWeaponIndex = 0;
    bool EquipAnimInprogress = false;
    
    void SpawnWeapons();
    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon,USceneComponent* SceneComponent, const FName& SocketName);
    void EquipWeapon( int32 WeaponIndex);
    
    void PlayAnimonatage(UAnimMontage* Animation);
    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    bool CanFire() const;
    bool CanEquip() const;
};
