// Shoot Them Up Game.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"



class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()
    
	
public:	
	ASTUBaseWeapon();
    FWeaponUIData GetUIData() const {return UIData; }
    FAmmoData GetAmmoData() const {return CurrentAmmo;}

    bool TryToAddAmmo(int32 ClipsAmount);

    FOnClipEmptySignature OnClipEmpty;

    virtual void StartFire();
    virtual void StopFire();

    void ChangeClip();
    bool CanReload() const;

protected:
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category="Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category="Weapon")
    float TraceMaxDistance = 1500;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Weapon")
    FAmmoData DefaultAmao =  {15,10,false};

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="UI")
    FWeaponUIData UIData;

    virtual void BeginPlay() override;
    
    virtual void MakeShot();
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
    APlayerController* GetPlayerController() const;
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    FVector GetMuzzleWorldLocation() const;

    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    void DecreaseAmmo();
    bool IsAmmoEmpty() const;
    bool IsClipEmpty() const;
    bool IsAmmoFull() const;
    
    void LogAmmo();
    

private:
    FAmmoData CurrentAmmo;
    


};
