// Shoot Them Up Game.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUPlayerStartRawWidget.generated.h"

class UImage;
class UTextBlock;
UCLASS()
class SHOOTTHEMUP_API USTUPlayerStartRawWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void SetPlayerTextBlock(const FText& Text);
    void SetkillsTextBlock(const FText& Text);
    void SetDeathTextBlock(const FText& Text);
    void SetTeamTextBlock(const FText& Text);
    void SetPlayerIndicatorImage(bool Visible);
    
protected:
    UPROPERTY(meta=(BindWidget))
    UTextBlock* PlayerNameTextBlock;

    UPROPERTY(meta=(BindWidget))
    UTextBlock* killsTextBlock;

    UPROPERTY(meta=(BindWidget))
    UTextBlock* DeathTextBlock;

    UPROPERTY(meta=(BindWidget))
    UTextBlock* TeamTextBlock;

    UPROPERTY(meta=(BindWidget))
    UImage* PlayerIndicatorImage;

    
	
};
