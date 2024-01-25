// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyHUD.h"
#include "Engine/Canvas.h"
#include "Characters/MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyHUD::AMyHUD() 
{
	CrosshairSize = 20.0f;
	CrosshairThickness = 2.0f;
    DistanceToCenter = DefaultDistanceToCenter;
    MoveDistanceToCenter = 60.f;
}

void AMyHUD::DrawHUD()
{
    Super::DrawHUD(); 
    
	DrawCrosshair();
    InterpCrosshair();
}

void AMyHUD::DrawCrosshair()
{ 
    //Return if not aiming
    if (!MyCharacter || MyCharacter->GetAimState() == false) { return; }
  
    // Get the screen size
    FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);

    // Center
    FVector2D Center = FVector2D(ScreenDimensions.X / 2, ScreenDimensions.Y / 2);

    // Horizontal line 
    DrawLine(Center.X + DistanceToCenter, Center.Y, Center.X + CrosshairSize + DistanceToCenter, Center.Y, FLinearColor::White, CrosshairThickness); //Right
    DrawLine(Center.X - DistanceToCenter, Center.Y, Center.X - CrosshairSize - DistanceToCenter, Center.Y, FLinearColor::White, CrosshairThickness); //Left


    // Vertical line
    DrawLine(Center.X, Center.Y - DistanceToCenter, Center.X, Center.Y - CrosshairSize - DistanceToCenter, FLinearColor::White, CrosshairThickness); //Top
    DrawLine(Center.X, Center.Y + DistanceToCenter, Center.X, Center.Y + CrosshairSize + DistanceToCenter, FLinearColor::White, CrosshairThickness); //Bottom

    // Dot in center
    // DrawRect(FLinearColor::White, Center.X - CrosshairThickness / 2, Center.Y - CrosshairThickness / 2, CrosshairThickness, CrosshairThickness);
}

void AMyHUD::InterpCrosshair()
{
    // Initialize MyCharacter
    if (APlayerController* PlayerController = GetOwningPlayerController())
    {
        MyCharacter = Cast<AMyCharacter>(PlayerController->GetPawn());
    }

    if (MyCharacter)
    {
        MyCharacterMovement = MyCharacter->GetCharacterMovement();
        GroundSpeed = UKismetMathLibrary::VSizeXY(MyCharacterMovement->Velocity);
    }

    if (GroundSpeed == 150.0)
    {
        DistanceToCenter = FMath::FInterpTo(DistanceToCenter, MoveDistanceToCenter, GetWorld()->GetDeltaSeconds(), 20.0f);
    }
    else
    {
        DistanceToCenter = FMath::FInterpTo(DistanceToCenter, DefaultDistanceToCenter, GetWorld()->GetDeltaSeconds(), 10.0f);
    }

    UE_LOG(LogTemp, Warning, TEXT("DistanceToCenter: %f"), DistanceToCenter);
    
}





