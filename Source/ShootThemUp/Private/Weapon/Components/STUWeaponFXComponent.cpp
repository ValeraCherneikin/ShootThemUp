// Shoot Them Up Game.All Rights Reserved.


#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"


USTUWeaponFXComponent::USTUWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
    auto ImpactData = DefaultImpactlData;

    if(Hit.PhysMaterial.IsValid())
    {
        const auto PhysMat = Hit.PhysMaterial.Get();
        if(ImpactlDataMap.Contains(PhysMat))
        {
            ImpactData = ImpactlDataMap[PhysMat];
        }
    }
    // niagara
        UNiagaraFunctionLibrary ::SpawnSystemAtLocation(GetWorld(), //
            ImpactData.NiagaraEffect,                              //
            Hit.ImpactPoint,                                      //
            Hit.ImpactNormal.Rotation());                        //

    // decal
    auto DecalComponent = UGameplayStatics ::SpawnDecalAtLocation(GetWorld(), //
        ImpactData.DecalData.Material,                 //
        ImpactData.DecalData.Size,                    // 
        Hit.ImpactPoint,                             //
        Hit.ImpactNormal.Rotation());
    if(DecalComponent)
    {
        DecalComponent ->SetFadeOut(ImpactData.DecalData.LifeTime,ImpactData.DecalData.FadeOutTime);
    }
    
}


