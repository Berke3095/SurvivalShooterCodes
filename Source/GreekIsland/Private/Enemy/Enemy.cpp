// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/TimerHandle.h"
//#include "Animation/AnimMontage.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 100;

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore); 
	
	if (bChasingCharacter)
	{
		GetCharacterMovement()->MaxWalkSpeed = 250.f;

		//Define physical animation data
		PhysicalAnimationData.bIsLocalSimulation = false;
		PhysicalAnimationData.OrientationStrength = 2000.f;
		PhysicalAnimationData.AngularVelocityStrength = 300.f;
		PhysicalAnimationData.PositionStrength = 2000.f;
		PhysicalAnimationData.VelocityStrength = 300.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 75.f;

		//Define physical animation data
		PhysicalAnimationData.bIsLocalSimulation = false;
		PhysicalAnimationData.OrientationStrength = 500.f;
		PhysicalAnimationData.AngularVelocityStrength = 100.f;
		PhysicalAnimationData.PositionStrength = 500.f;
		PhysicalAnimationData.VelocityStrength = 100.f;	 	
	}

	PhysicalAnimation = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnimationComponent"));

	//Get bone name
	Spine2 = FName(TEXT("Spine"));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
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

	if (GetCharacterMovement() && GetCharacterMovement()->Velocity.Size() > 0.0f) 
	{
		FVector VelocityDirection = GetCharacterMovement()->Velocity.GetSafeNormal(); //Move Direction
		FRotator TargetRotation = VelocityDirection.Rotation();   

		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationInterpSpeed); 

		FaceRotation(NewRotation, DeltaTime);
	}

	/*
	EnemySpeed = UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity);  

	if (GetCharacterMovement())
	{
		if (EnemySpeed == 0)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); 
			if (AnimInstance && EnemyIdleMontage) 
			{
				if (AnimInstance->Montage_IsPlaying(EnemyIdleMontage)) { return; }

				AnimInstance->Montage_Play(EnemyIdleMontage);
				int32 Selection = FMath::RandRange(0, 2);
				FName SectionName = FName();
				switch (Selection)
				{
				case 0:
					SectionName = FName("Idle");
					break;
				case 1:
					SectionName = FName("Agonizing");
					break;
				case 2:
					SectionName = FName("Eating");
					break;
				default:
					break;
				}
				AnimInstance->Montage_JumpToSection(SectionName, EnemyIdleMontage);
			}
		}
	}
	*/
}

void AEnemy::DestroyDeadEnemy()
{
	Destroy();
}

void AEnemy::ActivateRagdoll(FVector ImpulseDirection, FName HitBone)
{
	if (bIsRagdoll) { return; }
	
	GetCharacterMovement()->DisableMovement(); 

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->DestroyComponent();
	} 

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore); 

	GetMesh()->SetSimulatePhysics(true);

	// Apply impulse only to the specified bone
	GetMesh()->AddImpulse(ImpulseDirection * 1000, HitBone, true); 

	bIsRagdoll = true;

	if (PhysicalAnimation)
	{
		PhysicalAnimation->ConditionalBeginDestroy(); 
		PhysicalAnimation = nullptr; 
	}

	FTimerHandle DestroyHandler;
	GetWorld()->GetTimerManager().SetTimer(DestroyHandler, this, &AEnemy::DestroyDeadEnemy, 5.f); 
}

void AEnemy::HitReaction(FVector ImpulseDirection, FName HitBone)
{
	GetMesh()->AddImpulse(ImpulseDirection * BulletForce, HitBone, true);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

