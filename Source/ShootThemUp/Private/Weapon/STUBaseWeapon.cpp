// Shoot Them Up Game.All Rights Reserved.

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"



DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon,All,All)

ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    SetRootComponent(WeaponMesh);
    
}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
    check(WeaponMesh)
    CurrentAmmo = DefaultAmao;
}

void ASTUBaseWeapon::MakeShot(){}

void ASTUBaseWeapon::StartFire(){}

void ASTUBaseWeapon::StopFire(){}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if(!Player) return nullptr;

    return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLoacation, FRotator& ViewRotation) const
{
    const auto Controller = GetPlayerController();
    if(!Controller) return false;

    Controller->GetPlayerViewPoint(ViewLoacation,ViewRotation);
    return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLoacation;
    FRotator ViewRotation;
    if(!GetPlayerViewPoint(ViewLoacation,ViewRotation)) return false;

    TraceStart = ViewLoacation;
    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ASTUBaseWeapon:: MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if(!GetWorld()) return;
    
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    
    GetWorld()->LineTraceSingleByChannel(HitResult,TraceStart,TraceEnd,ECC_Visibility,CollisionParams);
}

void ASTUBaseWeapon::DecreaseAmmo()
{
    CurrentAmmo.Bullets --;
    LogAmmo();
    if(IsClipEmpty() && !IsAmmoEmpty())
    {
        ChangeClip();
    }
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

void ASTUBaseWeapon::ChangeClip()
{
    CurrentAmmo.Bullets = DefaultAmao.Bullets;
    if(!CurrentAmmo.Infinite)
    {
        CurrentAmmo.Clips--;
    }
    UE_LOG(LogBaseWeapon,Display,TEXT(" ----------Change Clip------------"));
}

void ASTUBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + "/";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(LogBaseWeapon,Display,TEXT("%s"),*AmmoInfo);
}
