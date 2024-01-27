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
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSound;
	
	//Bullet hole decal
	UPROPERTY(EditAnywhere);
	class UDecalComponent* DecalComponent;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* BulletHoleDecalMaterial;

	UFUNCTION()
	void SpawnBulletHoleDecal(const FHitResult& Hit);

	class AEnemy* Enemy;
	
public:	
	virtual void Tick(float DeltaTime) override;

};
