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

	class AMyCharacter* MyCharacter;

private:

	FName Spine2;

	FPhysicalAnimationData PhysicalAnimationData;

	void DestroyDeadEnemy(); 

	UPROPERTY(EditAnywhere)
	float RotationInterpSpeed = 2.f;

	FVector Distance; 

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* AttackMontage;

	class UAnimInstance* AnimInstance;

	void StopAttacking();
	FTimerHandle StopAttackHandler;

public:	

	float MaxHealth;
	float CurrentHealth;

	float EnemySpeed;

	float BulletForce = 2000.f;
	
	bool bChasingCharacter = true;

	FName HitBoneName;

	float DistanceInFloat;

	bool bIsAttacking;

	float EnemyPace;

	void ActivateRagdoll(FVector ImpulseDirection, FName HitBone);

	void HitReaction(FVector ImpulseDirection, FName HitBone);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
