// Shoot Them Up Game.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
    ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer);

protected:
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
    USpringArmComponent* SpringArmComponent;
    
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
    UCameraComponent* CameraComponent;

    virtual void BeginPlay() override;
    

public:	
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable,Category="Movement")
    bool bIsRunning()const;

    UFUNCTION(BlueprintCallable,Category="Movement")
    float GetMovementDirection()const;
private:
    bool bWantsToRun = false;
    bool bIsMovingForward = false;
    void MoveForward(float Amount);
    void MoveRight(float Amount);

    void OnStartRunning();
    void OnStopRunning();
    
};
