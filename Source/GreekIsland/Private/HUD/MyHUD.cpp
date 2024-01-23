// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyHUD.h"
#include "Engine/Canvas.h"

AMyHUD::AMyHUD() 
{
	CrosshairSize = 10.0f;
	CrosshairThickness = 2.0f;
}

void AMyHUD::DrawHUD()
{
	DrawCrosshair();
}

void AMyHUD::DrawCrosshair()
{
    // Get the screen dimensions
    FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);

    // Calculate the center of the screen
    FVector2D Center = FVector2D(ScreenDimensions.X / 2, ScreenDimensions.Y / 2);

    // Draw horizontal line
    DrawLine(Center.X - CrosshairSize, Center.Y, Center.X + CrosshairSize, Center.Y, FLinearColor::White, CrosshairThickness);

    // Draw vertical line
    DrawLine(Center.X, Center.Y - CrosshairSize, Center.X, Center.Y + CrosshairSize, FLinearColor::White, CrosshairThickness);

    // Draw a dot in the center
    DrawRect(FLinearColor::White, Center.X - CrosshairThickness / 2, Center.Y - CrosshairThickness / 2, CrosshairThickness, CrosshairThickness);
}



