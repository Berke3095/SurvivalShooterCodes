// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Characters/MyCharacter.h"
#include "Animation/AnimationAsset.h"
#include "Engine/SkeletalMeshSocket.h" 
#include "Weapons/Projectile.h"
#include "Weapons/BulletShell.h" 
#include "Combat/CombatComponent.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxAmmoClip = 30;

	//Weapon component placements
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); 
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); 
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);     
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);     

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay(); 
	
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);

		CurrentAmmo = MaxAmmoClip;  
	}
}

//Player overlapping the weapon sphere
void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter && PickupWidget && !MyCharacter->bCharacterDead)
	{
		PickupWidget->SetVisibility(true);
		MyCharacter->SetOverlappingWeapon(this);
	}
}

//Player leaving the sphere of weapon object
void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);  
	if (MyCharacter && PickupWidget)
	{
		PickupWidget->SetVisibility(false);
		MyCharacter->SetOverlappingWeapon(nullptr);
	}
}

//Equip the weapon
void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	WeaponMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

//Show widget
void AWeapon::ShowPickupWidget(bool bShowWidget)
{
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}

//Fire animations of the weapon / spawning shells
void AWeapon::Fire(const FVector& HitTarget)
{
	if (CurrentAmmo <= 0)
	{
		// Handle out of ammo condition
		return;
	}

	APawn* InstigatorPawn = Cast<APawn>(GetOwner()); //Player is the Instigator
	if (FireAnimation) 
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}
	if (BulletShellClass)
	{
		const USkeletalMeshSocket* AmmoEjectSocket = WeaponMesh->GetSocketByName(FName("AmmoEject"));
		if (BulletShellClass)
		{
			FTransform SocketTransform = AmmoEjectSocket->GetSocketTransform(WeaponMesh);  

			UWorld* World = GetWorld(); 
			if (World)
			{
				//Spawn the bullet
				World->SpawnActor<ABulletShell>(  
					BulletShellClass, 
					SocketTransform.GetLocation(), 
					SocketTransform.GetRotation().Rotator() 
				);
				SpendAmmo();
			}
		}
	}
	

	//Getting the location and rotation between trace end and start to spawn bullet at screen to the hitpoint
	CombatComponent = GetOwner()->FindComponentByClass<UCombatComponent>();
	if (CombatComponent)
	{
		FVector ToTarget = CombatComponent->TraceEnd - CombatComponent->TraceStart;
		FRotator TargetRotation = ToTarget.Rotation(); //Vector direction
		if (ProjectileClass && InstigatorPawn)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner(); //Player is the owner 
			SpawnParams.Instigator = InstigatorPawn;
			UWorld* World = GetWorld();
			if (World)
			{
				//Spawn the bullet
				World->SpawnActor<AProjectile>(
					ProjectileClass,
					CombatComponent->TraceStart,
					TargetRotation,
					SpawnParams
				);
			}
		}
	}
}

void AWeapon::SpendAmmo()
{
	CurrentAmmo--;
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("CurrentAmmo: %d"), CurrentAmmo);
}

