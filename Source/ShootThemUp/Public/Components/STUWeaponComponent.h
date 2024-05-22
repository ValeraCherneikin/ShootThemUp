// Shoot Them Up Game.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "Weapon/STUBaseWeapon.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUWeaponComponent();

    virtual void StartFire();
    void StopFire();
    virtual void NextWeapon();
    void Reload();

    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

    bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount);

protected:
    UPROPERTY(EditDefaultsOnly,Category = "Weapon")
    TArray<FWeaponData> WeaponData;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    bool CanFire() const;
    bool CanEquip() const;
    void EquipWeapon( int32 WeaponIndex);

    UPROPERTY(EditDefaultsOnly,Category = "Weapon")
    FName  WeaponEquipSocketName = "WeaponSocket";

    UPROPERTY(EditDefaultsOnly,Category = "Weapon")
    FName  WeaponArmorySocketName = "ArmorySocket";

    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;
    
    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;

    int32 CurrentWeaponIndex = 0;

    UPROPERTY(EditDefaultsOnly,Category = "Animation")
    UAnimMontage* EquipAnimMontage;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    
    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;
    
    bool EquipAnimInprogress = false;
    bool ReloadAnimInprogress = false;
    
    void SpawnWeapons();
    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon,USceneComponent* SceneComponent, const FName& SocketName);
    
    void PlayAnimonatage(UAnimMontage* Animation);
    void InitAnimations();

    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);
    
    bool CanReload() const;

    void OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon);
    void ChangeClip();
    template<typename T>
    T* FindNotifyByClass(UAnimSequenceBase* Animation)
    {
        if(!Animation) return nullptr;

        const auto NotyfyEvents = Animation -> Notifies;
        for(auto NotyfyEvent: NotyfyEvents)
        {
            auto AnimNotify = Cast<T>(NotyfyEvent.Notify);
            if(AnimNotify)
            {
                return AnimNotify;
            }
        }
        return nullptr;
    };
};
