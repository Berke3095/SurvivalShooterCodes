// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Projectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/DecalComponent.h"
#include "Enemy/Enemy.h"
#include "Engine/TimerHandle.h"
#include "Components/CapsuleComponent.h"

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
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);

	//Bullet speed
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	
	//Bullet decal
	BulletDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("BulletHoleDecal"));
	BulletDecalComponent->SetupAttachment(RootComponent); 
	BulletDecalComponent->DecalSize = FVector(5.0f, 5.0f, 5.0f); 
	BulletDecalComponent->SetFadeScreenSize(0.f);
	if (BulletDecalComponent)
	{
		BulletDecalComponent->SetDecalMaterial(BulletHoleDecalMaterial); 
	}

	//Blood decal
	BloodDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("BloodHoleDecal"));
	BloodDecalComponent->SetupAttachment(RootComponent);
	BloodDecalComponent->DecalSize = FVector(5.0f, 5.0f, 5.0f);
	BloodDecalComponent->SetFadeScreenSize(0.f);
	if (BloodDecalComponent)
	{
		BloodDecalComponent->SetDecalMaterial(BloodHoleDecalMaterial); 
	}

	//Stain decal
	BloodStainDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("BloodStainDecal"));
	BloodStainDecalComponent->SetupAttachment(RootComponent);
	BloodStainDecalComponent->DecalSize = FVector(40.0f, 40.0f, 40.0f);
	BloodStainDecalComponent->SetFadeScreenSize(0.f);

	BloodStainMaterials.SetNum(8); 
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
	{
		bEnemyHit = true;

		FVector ImpulseDirection = Hit.ImpactPoint - GetActorLocation();
		ImpulseDirection.Normalize();

		Enemy->HitBoneName = Hit.BoneName;

		DealDamage(Damage, ImpulseDirection);
		if (ZombieHitParticles)
		{ 
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ZombieHitParticles, GetActorTransform()); 

			if (BloodStainMaterials[0])
			{
				UE_LOG(LogTemp, Warning, TEXT("Stainmats online"));
				SpawnStainDecal();
			}
		}

		if (ZombieHitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ZombieHitSound, GetActorLocation());
		}
		
		
		if (BloodHoleDecalMaterial)
		{
			SpawnBloodHoleDecal(Hit);
		}
	}
	else 
	{
		if (StoneHitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), StoneHitParticles, GetActorTransform());
		}

		if (StoneHitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, StoneHitSound, GetActorLocation());
		}

		if (BulletHoleDecalMaterial)
		{
			SpawnBulletHoleDecal(Hit);
		}
	}

	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AProjectile::DestroyProjectile, .2f, false);
}

void AProjectile::SpawnBloodHoleDecal(const FHitResult& Hit)
{
	if (BloodHoleDecalMaterial)
	{
		FVector DecalSize = FVector(5.0f, 5.0f, 5.0f);
		FRotator DecalRotation = Hit.ImpactNormal.Rotation();

		// Spawn the blood hole decal at the hit location
		UGameplayStatics::SpawnDecalAttached(
			BloodHoleDecalMaterial,
			DecalSize,
			Enemy->GetMesh(),
			Enemy->HitBoneName,
			Hit.ImpactPoint,
			DecalRotation,
			EAttachLocation::KeepWorldPosition,
			10.f
		)->SetFadeScreenSize(0.f);
	}
}

void AProjectile::SpawnBulletHoleDecal(const FHitResult& Hit)
{
	if (BulletHoleDecalMaterial)
	{
		FVector DecalSize = FVector(5.0f, 5.0f, 5.0f); 
		FRotator DecalRotation = Hit.ImpactNormal.Rotation(); 

		// Spawn the bullet hole decal at the hit location
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletHoleDecalMaterial, DecalSize, Hit.ImpactPoint, DecalRotation, 10.0f)->SetFadeScreenSize(0.f);
	}
}

void AProjectile::DestroyProjectile()
{
	Destroy();
}

void AProjectile::SpawnStainDecal() 
{
	if (BloodStainMaterials[0])
	{
		FVector EnemyBottomLocation = Enemy->GetActorLocation() - FVector(0.f, 0.f, Enemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

		float RandomXOffset = FMath::RandRange(-15.f, 15.f);
		FVector LocalXAxis = Enemy->GetActorRotation().RotateVector(FVector::RightVector); 
		FVector LocalYAxis = Enemy->GetActorRotation().RotateVector(FVector::ForwardVector);  

		FVector YOffset = LocalYAxis * 50.f;
		FVector XOffset = LocalXAxis * RandomXOffset; 

		FVector ModifiedLocation = EnemyBottomLocation + YOffset + RandomXOffset; 

		int32 RandomIndex = FMath::RandRange(0, 7);
		// Spawn the blood stain decal at the hit location
		UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			BloodStainMaterials[RandomIndex],
			FVector(20.0f, 20.0f, 20.0f),
			ModifiedLocation,
			FRotator(90.f, 0, 0), 
			10.0f
		)->SetFadeScreenSize(0.f);
	}
}

void AProjectile::DealDamage(float DamageValue, FVector ImpulseDirection)
{
	if (Enemy)
	{
		Enemy->CurrentHealth -= DamageValue;

		if (Enemy->CurrentHealth > 0)
		{
			Enemy->HitReaction(ImpulseDirection, Enemy->HitBoneName);
		}

		if (Enemy->CurrentHealth <= 0)
		{
			Enemy->ActivateRagdoll(ImpulseDirection, Enemy->HitBoneName); 
		}
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



