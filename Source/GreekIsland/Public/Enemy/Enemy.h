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

	UPROPERTY() 
	class AMyCharacter* MyCharacter;

	UPROPERTY()
	class AAIController* EnemyController;

	FPhysicalAnimationData PhysicalAnimationData;  

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
private:

	FName Spine2;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* AttackMontage;

	class UAnimInstance* AnimInstance;

	void StopAttacking();
	FTimerHandle StopAttackHandler;

	UFUNCTION(BlueprintCallable)
	void EnableCollision(); 
	UFUNCTION(BlueprintCallable)
	void DisableCollision(); 
	UFUNCTION(BlueprintCallable)
	void SetHasDamaged(bool BoolValue);

public:	
	float MaxHealth;
	float CurrentHealth;

	float BulletForce = 2000.f;
	
	bool bChasingCharacter = true;

	FName HitBoneName;

	UPROPERTY(BlueprintReadOnly)
	float DistanceInFloat;

	UPROPERTY(BlueprintReadOnly)
	bool bIsAttacking;

	UPROPERTY(BlueprintReadOnly)
	float EnemyPace;

	void ActivateRagdoll(FVector ImpulseDirection, FName HitBone);

	void HitReaction(FVector ImpulseDirection, FName HitBone);

	void EnemyDealDamage(float DamageValue);

	UPROPERTY(EditAnywhere, Category = "Combat")
	float EnemyDamage = 20;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
