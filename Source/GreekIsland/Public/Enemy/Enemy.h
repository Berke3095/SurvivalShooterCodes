// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class GREEKISLAND_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class UEnemyAnimInstance* EnemyAnimInstance;

	class UPhysicalAnimationComponent* PhysicalAnimation;

	bool bIsRagdoll;

private:

	FName Spine2;

	FPhysicalAnimationData PhysicalAnimationData;

public:	

	float MaxHealth;
	float CurrentHealth;
	
	bool bChasingCharacter;
	bool bIsHit;

	FName HitBoneName;

	void ActivateRagdoll(FVector ImpulseDirection, FName HitBone);

	void HitReaction(FVector ImpulseDirection, FName HitBone);
	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
