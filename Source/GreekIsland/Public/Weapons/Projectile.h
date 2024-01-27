// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

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

private:

	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	//Bullet pace
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	//particles and sound of impact
	UPROPERTY(EditAnywhere, Category = "Effects")
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class USoundCue* ImpactSound;
	
	//Bullet hole decal
	UPROPERTY(EditAnywhere, Category = "Decal");
	class UDecalComponent* DecalComponent;

	UPROPERTY(EditAnywhere, Category = "Decal")
	class UMaterialInterface* BulletHoleDecalMaterial;

	UFUNCTION()
	void SpawnBulletHoleDecal(const FHitResult& Hit); 

	class AEnemy* Enemy;  

public:	

	void DealDamage(float DamageValue, FVector ImpulseDirection);

	UPROPERTY(EditAnywhere, Category = "Combat Stats")
	float Damage = 10;

	virtual void Tick(float DeltaTime) override;

};
