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
#include "Characters/MyCharacter.h"
#include "NiagaraFunctionLibrary.h"

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
	BloodStainDecalComponent->DecalSize = FVector(3.0f, 30.0f, 30.0f);
	BloodStainDecalComponent->SetFadeScreenSize(0.f);

	BloodStainMaterials.SetNum(8);   
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	PlayerCharacter = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()); 
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Projectile hit
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
	{
		bEnemyHit = true;

		FVector ImpulseDirection = Hit.ImpactPoint - GetActorLocation();
		ImpulseDirection.Normalize();

		Enemy->HitBoneName = Hit.BoneName;

		if (Enemy->HitBoneName == TEXT("Head"))
		{
			DealDamage(Damage * CritMultiplier, ImpulseDirection); 

			if (ZombieHeadshotParticles && Enemy->CurrentHealth <= 0)
			{
				Enemy->GetMesh()->HideBoneByName(TEXT("Head"), PBO_None);

				FName SocketName = TEXT("SplashSocket");
				if (Enemy->GetMesh()->DoesSocketExist(SocketName))
				{
					FTransform SocketTransform = Enemy->GetMesh()->GetSocketTransform(SocketName); 
					UGameplayStatics::SpawnEmitterAttached(
						ZombieHeadshotParticles, 
						Enemy->GetMesh(),
						SocketName,
						FVector::ZeroVector,
						FRotator::ZeroRotator,
						EAttachLocation::SnapToTarget,
						false
					);
				}

				if (BloodStainMaterials[0])
				{
					SpawnStainDecalEnemy();
				}

				if (ZombieHeadshotSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, ZombieHeadshotSound, GetActorLocation()); 
				}

				if (BloodHoleDecalMaterial)
				{
					SpawnBloodHoleDecal(Hit);
				}

				//NeckSocket check
				FName NiagaraSocket = TEXT("NeckSocket");
				if (Enemy->GetMesh()->DoesSocketExist(NiagaraSocket)) 
				{
					FTransform SocketTransform = Enemy->GetMesh()->GetSocketTransform(NiagaraSocket); 
					// Spawn Niagara effect on the neck bone
					UNiagaraFunctionLibrary::SpawnSystemAttached( 
						NeckBloodSpray,
						Enemy->GetMesh(), 
						NiagaraSocket, 
						SocketTransform.GetLocation(), 
						SocketTransform.Rotator(),
						EAttachLocation::KeepWorldPosition, 
						false
					);
				} 
			}
		}
		else
		{
			DealDamage(Damage, ImpulseDirection);

			if (ZombieHitParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ZombieHitParticles, GetActorTransform());

				if (BloodStainMaterials[0])
				{
					SpawnStainDecalEnemy();
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
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AProjectile::DestroyProjectile, .1f, false);
}

//Spawning bullet holes on zombies
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

//Spawning bullet holes on walls
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

//Destroying the bullet after the hit
void AProjectile::DestroyProjectile()
{
	Destroy();
}

//Spawning blood stains on the floor towards the player (Random)
void AProjectile::SpawnStainDecalEnemy()
{
	if (BloodStainMaterials[0] && Enemy && GetWorld()->GetFirstPlayerController())
	{	 
		if (PlayerCharacter) 
		{
			FVector PlayerLocation = PlayerCharacter->GetActorLocation() - FVector(0.f, 0.f, PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
			FVector EnemyLocation = Enemy->GetActorLocation() - FVector(0.f, 0.f, Enemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

			// Calculate the direction vector from player to enemy
			//FVector EnemyBackDirection = (EnemyLocation - PlayerLocation).GetSafeNormal();
			FVector EnemyFrontDirection = (PlayerLocation - EnemyLocation).GetSafeNormal();

			float RandomXOffset = FMath::RandRange(-20.f, 20.f);
			FVector LocalXAxis = Enemy->GetActorRotation().RotateVector(FVector::RightVector);

			FVector XOffset = LocalXAxis * RandomXOffset;

			// Spawn the blood stain decal along the direction from player to enemy
			FVector ModifiedLocation = EnemyLocation + EnemyFrontDirection * 80.f + XOffset;
			FRotator StainRotation = (EnemyFrontDirection.Rotation() + FRotator(90.f, 0.f, 0.f));

			int8 RandomIndex = FMath::RandRange(0, 7); 

			float SpawnTime = GetWorld()->GetTimeSeconds(); 
			float ElapsedTime = GetWorld()->GetTimeSeconds() - SpawnTime; 

			FVector StainSize = FVector(3.0f, 30.0f, 30.0f);

			UDecalComponent* StainDecal = UGameplayStatics::SpawnDecalAtLocation(
				GetWorld(),
				BloodStainMaterials[RandomIndex],
				StainSize, 
				ModifiedLocation,
				StainRotation,
				10.0f
			);

			StainDecal->SetFadeScreenSize(0.f); 
		}
	}
}

//Projectiles dealing damage to zombies
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
			PlayerCharacter->SetKillCount();
		}
	}
}





