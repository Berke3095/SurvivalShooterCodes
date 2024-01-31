// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 400;

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore); 
	
	if (bChasingCharacter)
	{
		GetCharacterMovement()->MaxWalkSpeed = 250.f;
	}
	else{ GetCharacterMovement()->MaxWalkSpeed = 75.f; }

	PhysicalAnimation = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnimationComponent"));

	//Get bone name
	Spine2 = FName(TEXT("Spine")); 

	//Define physical animation data
	PhysicalAnimationData.bIsLocalSimulation = false;
	PhysicalAnimationData.OrientationStrength = 100.f; 
	PhysicalAnimationData.AngularVelocityStrength = 100.f; 
	PhysicalAnimationData.PositionStrength = 500.f; 
	PhysicalAnimationData.VelocityStrength = 100.f; 
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	if (PhysicalAnimation)
	{
		PhysicalAnimation->SetSkeletalMeshComponent(GetMesh());
		PhysicalAnimation->ApplyPhysicalAnimationSettingsBelow(Spine2, PhysicalAnimationData);
		if (GetMesh())
		{
			GetMesh()->SetAllBodiesBelowSimulatePhysics(Spine2, true);
		}
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::ActivateRagdoll(FVector ImpulseDirection, FName HitBone)
{
	if (bIsRagdoll) { return; }

	GetMesh()->SetSimulatePhysics(true);

	// Apply impulse only to the specified bone
	GetMesh()->AddImpulse(ImpulseDirection * 2000.f, HitBone, true);

	bIsRagdoll = true;

	if (PhysicalAnimation)
	{
		PhysicalAnimation->ConditionalBeginDestroy(); 
		PhysicalAnimation = nullptr; // Set to nullptr to avoid using a dangling pointer
	}
}


void AEnemy::HitReaction(FVector ImpulseDirection, FName HitBone)
{
	GetMesh()->AddImpulse(ImpulseDirection * 4000.f, HitBone, true);
}


// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

