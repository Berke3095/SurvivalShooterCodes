// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GREEKISLAND_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	//Animation initialization and update events
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly)
	class AEnemy* Enemy;

	class UCharacterMovementComponent* EnemyMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float EnemySpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsChasing;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bEnemyIsHit;

	UPROPERTY(BlueprintReadOnly, Category = Combat)
	bool bIsAttackingAnim;
};
