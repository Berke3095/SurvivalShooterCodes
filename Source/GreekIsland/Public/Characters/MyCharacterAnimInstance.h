// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/CharacterStates.h"
#include "MyCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GREEKISLAND_API UMyCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	//Animation initialization and update events
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	//Creating character reference
	UPROPERTY(BlueprintReadOnly)
	class AMyCharacter* MyCharacter;

	//Creating character movement reference
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* MyCharacterMovement;

	UPROPERTY(BlueprintReadOnly)
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float CharacterDirection;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float CharacterPitchRotation;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float CharacterYawRotation;

	UPROPERTY(BlueprintReadOnly, Category = Movement) 
	float YawChangeRate;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float PreviousYaw;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float CurrentYaw;

	//UPROPERTY(BlueprintReadOnly, Category = Movement) 
	//float CharacterYawRotateValue;

	//Falling state
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsFalling;

	//Just jumped state
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bJustJumped;
	
	//Aim state
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bAimingAnim;

	//Firing state
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bFiringAnim;

public:

	bool GetbIsFalling() const { return bIsFalling; }

	FORCEINLINE bool SetbJustJumped(bool BoolValue) { return bJustJumped = BoolValue; }
	bool GetbJustJumped() const { return bJustJumped; }
};
