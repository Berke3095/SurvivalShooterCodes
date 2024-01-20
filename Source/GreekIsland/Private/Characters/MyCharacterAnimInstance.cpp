// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MyCharacterAnimInstance.h"
#include "Characters/MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"


//Animation initialization and update events
void UMyCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//Initialize MyCharacter script
	MyCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
	if (MyCharacter)
	{
		MyCharacterMovement = MyCharacter->GetCharacterMovement();
	}

}

void UMyCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MyCharacterMovement)
	{
		CharacterDirection = UKismetAnimationLibrary::CalculateDirection(MyCharacterMovement->Velocity, MyCharacter->GetActorRotation());
		GroundSpeed = UKismetMathLibrary::VSizeXY(MyCharacterMovement->Velocity);

		//Aim offset degrees
		FRotator AimRotation = MyCharacter->GetBaseAimRotation();
		float ClampedPitch = FMath::Clamp(AimRotation.Pitch, -90.f, 90.f);
		CharacterPitchRotation = ClampedPitch;
		
		float ClampedYaw = FMath::Clamp(AimRotation.Yaw,-180.f, 180.f);
		CharacterYawRotation = ClampedYaw; 
		//CharacterYawRotateValue = AimRotation.Yaw; 

		// Calculate the change in yaw over the last second
		CurrentYaw = MyCharacter->GetBaseAimRotation().Yaw; 
		YawChangeRate = (CurrentYaw - PreviousYaw) / DeltaTime; 

		// Store the current yaw for the next frame
		PreviousYaw = CurrentYaw;

		bIsFalling = MyCharacterMovement->IsFalling();
		CharacterState = MyCharacter->GetCharacterState(); 
		bAimingAnim = MyCharacter->GetAimState();
		bFiringAnim = MyCharacter->GetFireState();
	}
}