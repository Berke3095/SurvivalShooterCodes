// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Projectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/DecalComponent.h"
#include "Enemy/Enemy.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creating the bullet collision
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	//Bullet speed
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	
	//Bullet decal
	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("BulletHoleDecal"));
	DecalComponent->SetupAttachment(RootComponent); 
	DecalComponent->DecalSize = FVector(5.0f, 5.0f, 5.0f); 
	if (DecalComponent)
	{
		DecalComponent->SetDecalMaterial(BulletHoleDecalMaterial); 
	}
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation()); 
	}
	
	if (BulletHoleDecalMaterial)
	{
		SpawnBulletHoleDecal(Hit);
	}

	Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
	{
		FVector ImpulseDirection = Hit.ImpactPoint - GetActorLocation();
		ImpulseDirection.Normalize();

		DealDamage(Damage, ImpulseDirection);

		UE_LOG(LogTemp, Warning, TEXT("EnemyCurrentHealth: %f"), Enemy->CurrentHealth);
	}
	
	Destroy();
}

void AProjectile::SpawnBulletHoleDecal(const FHitResult& Hit)
{
	if (BulletHoleDecalMaterial)
	{
		FVector DecalSize = FVector(5.0f, 5.0f, 5.0f); 
		FRotator DecalRotation = Hit.ImpactNormal.Rotation(); 

		// Spawn the bullet hole decal at the hit location
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletHoleDecalMaterial, DecalSize, Hit.ImpactPoint, DecalRotation, 5.0f); 
	}
}

void AProjectile::DealDamage(float DamageValue, FVector ImpulseDirection)
{
	if (Enemy)
	{
		Enemy->CurrentHealth -= DamageValue;

		if (Enemy->CurrentHealth <= 0)
		{
			Enemy->ActivateRagdoll(ImpulseDirection);
		}
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

