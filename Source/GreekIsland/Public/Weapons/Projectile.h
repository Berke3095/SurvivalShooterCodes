// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UMaterialInterface;

UCLASS()
class GREEKISLAND_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	//particles and sound of impact
	UPROPERTY(EditAnywhere, Category = "Effects")
	class UParticleSystem* StoneHitParticles;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class UParticleSystem* ZombieHitParticles;

	//Bullet hole decal
	UPROPERTY(EditAnywhere, Category = "Decal")
	class UDecalComponent* BloodDecalComponent;

	UPROPERTY(EditAnywhere, Category = "Decal")
	UMaterialInterface* BloodHoleDecalMaterial;

	UFUNCTION()
	void SpawnBloodHoleDecal(const FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category = "Effects")
	class USoundCue* StoneHitSound;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class USoundCue* ZombieHitSound;

	//Bullet hole decal
	UPROPERTY(EditAnywhere, Category = "Decal")
	class UDecalComponent* BulletDecalComponent;

	UPROPERTY(EditAnywhere, Category = "Decal")
	UMaterialInterface* BulletHoleDecalMaterial;

	UFUNCTION()
	void SpawnBulletHoleDecal(const FHitResult& Hit);

	class AEnemy* Enemy;

	void DestroyProjectile();

	UPROPERTY(EditAnywhere, Category = "Decal")
	class UDecalComponent* BloodStainDecalComponent;

	UPROPERTY(EditAnywhere, Category = "Decal")
	TArray<UMaterialInterface*> BloodStainMaterials;

	UFUNCTION()
	void SpawnStainDecal();

	//Bullet pace
	UPROPERTY(VisibleAnywhere) 
	class UProjectileMovementComponent* ProjectileMovementComponent; 

private:

public:	

	void DealDamage(float DamageValue, FVector ImpulseDirection);

	UPROPERTY(EditAnywhere, Category = "Combat Stats")
	float Damage = 10;

	bool bEnemyHit = false;
	
	virtual void Tick(float DeltaTime) override;

};
