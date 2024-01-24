// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class GREEKISLAND_API AMyHUD : public AHUD
{
	GENERATED_BODY()
	
public:
    // Constructor(Defaults)
    AMyHUD();

    // Override the DrawHUD function
    virtual void DrawHUD() override;

private:
    // Function to draw the crosshair
    void DrawCrosshair();

    //Dynamic crosshair
    void InterpCrosshair();

    float CrosshairSize;
    float CrosshairThickness;
    float DefaultDistanceToCenter = 15.f;
    float DistanceToCenter;
    float MoveDistanceToCenter;

    float GroundSpeed;

    class AMyCharacter* MyCharacter;

    class UCharacterMovementComponent* MyCharacterMovement;
};
