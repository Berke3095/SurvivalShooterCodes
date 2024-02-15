// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

//Set health
void UMyOverlay::SetHealthBarPercent(float Percent) 
{
    if (HealthBar)
    {
        HealthBar->SetPercent(Percent);
    }
}
//SetStamina
void UMyOverlay::SetStaminaBarPercent(float Percent) 
{
    if (StaminaBar)
    {
        StaminaBar->SetPercent(Percent);
    }
}
//Set kill count
void UMyOverlay::SetKillCount(int32 ZombieKillCount) 
{
    if (DeathCount)
    {
        DeathCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), ZombieKillCount)));
    }
}

void UMyOverlay::SetAmmoCount(int32 CurrentAmmo)
{
    if (AmmoCount) 
    {
        AmmoCount->SetText(FText::FromString(FString::Printf(TEXT("Ammo: %d / 30"), CurrentAmmo))); 
    }
}
