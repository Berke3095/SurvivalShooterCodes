// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyOverlay.generated.h"

/**
 * 
 */
UCLASS()
class GREEKISLAND_API UMyOverlay : public UUserWidget
{
	GENERATED_BODY()
	
private:

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* StaminaBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* DeathCount;

public:

    //Setting hud dynamics 
    FORCEINLINE void SetHealthBarPercent(float Percent); 
    FORCEINLINE void SetStaminaBarPercent(float Percent); 
    FORCEINLINE void SetKillCount(int32 ZombieKillCount); 
};
