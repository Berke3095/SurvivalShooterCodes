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

protected:

    virtual void BeginPlay() override;

private:

    // Function to draw the crosshair
    void DrawCrosshair(); 

    //Dynamic crosshair
    void InterpCrosshair();

    float CrosshairSize;
    float CrosshairThickness;
    
    float GroundSpeed;

    class AMyCharacter* MyCharacter;

    class UCharacterMovementComponent* MyCharacterMovement;

    TArray<AActor*> FoundActors;  

    UPROPERTY(EditDefaultsOnly, Category = Hud)
    TSubclassOf<class UMyOverlay> MyOverlayClass;

    UPROPERTY()
    class UMyOverlay* MyOverlay; 

public:
    float DistanceToCenter;
    float DefaultDistanceToCenter = 15.f;
    float MoveDistanceToCenter;
    float HitIndicatorDistance;
    float HitIndicatorSize;
    float HitIndicatorThickness;

    //Hit indicator
    FORCEINLINE void DrawHit(); 

    FORCEINLINE UMyOverlay* GetMyOverlay() const { return MyOverlay; }

    
};
