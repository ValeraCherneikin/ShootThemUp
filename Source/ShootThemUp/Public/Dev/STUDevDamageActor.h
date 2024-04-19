// Shoot Them Up Game.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUDevDamageActor.generated.h"



UCLASS()
class SHOOTTHEMUP_API ASTUDevDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUDevDamageActor();

    UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
    USceneComponent* SceneComponent;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    float Radius = 300.0f;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    float Damage = 10.0f;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    bool bDoFullDamage = false;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    FColor SpereColor = FColor::Red;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TSubclassOf<UDamageType> DamageType;
    

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};