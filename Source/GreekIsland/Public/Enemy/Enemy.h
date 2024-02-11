// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CharacterStates.h" 
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

	bool bIsRagdoll; 

	UPROPERTY() 
	class AMyCharacter* MyCharacter;

	UPROPERTY()
	class AAIController* EnemyController;

	void DestroyDeadEnemy();

	UPROPERTY(EditAnywhere)
	float RotationInterpSpeed = 2.f;

	FVector Distance; 

	UPROPERTY(EditAnywhere)
	class UBoxComponent* RightHandComponent;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* LeftHandComponent;

	UFUNCTION()
	void OnRightHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLeftHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ChasePlayer();

	bool bHasDamaged = false;
	bool bCollisionOn = false;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundCue* ZombieSound;

	void DelayRagdoll();

	UPROPERTY(BlueprintReadOnly) 
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

private:

	class UPhysicalAnimationComponent* PhysicalAnimation; 

	FPhysicalAnimationData PhysicalAnimationData; 

	FName Spine2;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* DeathMontage;

	bool bDeathAnimPlayed = false;

	class UAnimInstance* AnimInstance;

	FTimerHandle StopAttackHandler; 
	void StopAttacking(); 
	
	//Attack collisions set
	UFUNCTION(BlueprintCallable)
	void EnableCollision(); 
	UFUNCTION(BlueprintCallable)
	void DisableCollision(); 
	UFUNCTION(BlueprintCallable)
	void SetHasDamaged(bool BoolValue);
	
	bool bSoundPlaying = false; 
	void ResetSound();
	FTimerHandle SoundResetTimer;

public:	
	const float MaxHealth = 100;
	float CurrentHealth;

	const float BulletForce = 2000.f;
	
	bool bChasingCharacter = true;

	FName HitBoneName;

	UPROPERTY(BlueprintReadOnly)
	float DistanceInFloat;

	UPROPERTY(BlueprintReadOnly)
	bool bIsAttacking;

	UPROPERTY(BlueprintReadOnly)
	float EnemyPace;

	//Activating ragdoll as the enemy dies
	FORCEINLINE void ActivateRagdoll(FVector ImpulseDirection, FName HitBone); 

	//Impulse on the shot bone
	FORCEINLINE void HitReaction(FVector ImpulseDirection, FName HitBone); 

	//Enemy dealing damage
	FORCEINLINE void EnemyDealDamage(float DamageValue);

	//Assigning damage to enemy attacks
	UPROPERTY(EditAnywhere, Category = "Combat")
	float EnemyDamage = 20;

	bool bZombieDead = false; 
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
