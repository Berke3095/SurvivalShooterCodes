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

	UPROPERTY(EditAnywhere)
	class UBoxComponent* RightFootComponent;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* LeftFootComponent;

	UFUNCTION()
	void OnRightHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLeftHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLeftFootOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnRightFootOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ChasePlayer();

	bool bCollisionOn = false;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundCue* ZombieSound;

	void DelayRagdoll();

	UPROPERTY(BlueprintReadOnly) 
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
	float MaxWalkSpeed = 250.f;

	UPROPERTY(BlueprintReadOnly, Category = "BodyPart")
	FName Spine2; 

	void SetDodgeFalse();

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackRadius = 150.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float AttackCooldownDuration = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float AttackCooldownDuration_Leap = 15.f;

	float TimeSinceLastAttack; 
	float TimeSinceLastAttack_Leap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float AcceptanceRadius = 15.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float LeapRadius = 800.f;

	class AEnemySpawner* EnemySpawner;

private:

	class UPhysicalAnimationComponent* PhysicalAnimation; 

	FPhysicalAnimationData PhysicalAnimationData; 

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* DodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* LeapMontage;

	bool bDeathAnimPlayed = false;

	class UAnimInstance* AnimInstance;

	FTimerHandle StopAttackHandler; 
	void StopAttacking(); 
	
	//Attack collisions set
	UFUNCTION(BlueprintCallable)
	void EnableCollisionHands(); 
	UFUNCTION(BlueprintCallable)
	void DisableCollisionHands();  

	UFUNCTION(BlueprintCallable)
	void EnableCollisionFeet(); 
	UFUNCTION(BlueprintCallable)
	void DisableCollisionFeet(); 
	
	bool bSoundPlaying = false; 
	void ResetSound();
	FTimerHandle SoundResetTimer;

public:	
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxHealth = 100;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
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

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	bool bHasDamaged = false; 

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

	bool bEnemyCanDodge = false;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
