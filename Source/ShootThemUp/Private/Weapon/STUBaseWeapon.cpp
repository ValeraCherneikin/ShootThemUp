// Shoot Them Up Game.All Rights Reserved.

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Viewports.h"
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
}

void ASTUBaseWeapon::MakeShot()
{
    if(!GetWorld()) return;

    const auto Player = Cast<ACharacter>(GetOwner());
    if(!Player) return;

    const auto Controller = Player->GetController<APlayerController>();
    if(!Controller) return;

    FVector ViewLoacation;
    FRotator ViewRotation;
    Controller->GetPlayerViewPoint(ViewLoacation,ViewRotation);

    const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
    const FVector TraceStart = ViewLoacation;            //SocketTransform.GetLocation();
    const FVector ShootDirection = ViewRotation.Vector();//SocketTransform.GetRotation().GetForwardVector();
    const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    GetWorld()->LineTraceSingleByChannel(HitResult,TraceStart,TraceEnd,ECC_Visibility,CollisionParams);

    if(HitResult.bBlockingHit)
    {
        DrawDebugLine(GetWorld(),SocketTransform.GetLocation(), HitResult.ImpactPoint,FColor::Red,false,3.0f,0,3.0f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint,10.0f,24,FColor::Red,false,5.0f);

        UE_LOG(LogBaseWeapon,Display,TEXT("Bone: %s"), *HitResult.BoneName.ToString());
    } else
    {
        DrawDebugLine(GetWorld(),SocketTransform.GetLocation(), TraceEnd,FColor::Red,false,3.0f,0,3.0f);
    }
}

void ASTUBaseWeapon::Fire()
{
    UE_LOG(LogBaseWeapon,Display,TEXT("Fire!!!"));
    MakeShot();
}

