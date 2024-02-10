// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/TimerHandle.h"
#include "Characters/MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "AIController.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RightHandComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandComponent"));
	RightHandComponent->SetupAttachment(GetMesh(), FName(TEXT("RightHand")));
	RightHandComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightHandComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);

	LeftHandComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandComponent"));
	LeftHandComponent->SetupAttachment(GetMesh(), FName(TEXT("LeftHand")));
	RightHandComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);  
	LeftHandComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftHandComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore); 

	PhysicalAnimation = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnimationComponent")); 

	//Get bone name
	Spine2 = FName(TEXT("Spine")); 
	
	EnemyPace = GetCharacterMovement()->Velocity.Size(); 

	if (bChasingCharacter && !bIsAttacking)
	{
		//Strong body
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
		//Weak body
		GetCharacterMovement()->MaxWalkSpeed = 75.f;

		PhysicalAnimationData.bIsLocalSimulation = false;
		PhysicalAnimationData.OrientationStrength = 1000.f;
		PhysicalAnimationData.AngularVelocityStrength = 100.f;
		PhysicalAnimationData.PositionStrength = 1000.f;
		PhysicalAnimationData.VelocityStrength = 100.f;
	}

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	RightHandComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnRightHandOverlap); 
	LeftHandComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnLeftHandOverlap); 

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

	FTimerHandle MovementUpdateTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(MovementUpdateTimerHandle, this, &AEnemy::ChasePlayer, 0.5f, true); 
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentHealth <= 0)
	{
		bZombieDead = true;

		ZombieSound = nullptr;
	}
	else { bZombieDead = false; }

	if (!bZombieDead)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Attacking: %s"), bIsAttacking ? TEXT("true") : TEXT("false")); 

		if (ZombieSound && !bSoundPlaying && !bIsAttacking)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ZombieSound, GetActorLocation());
			bSoundPlaying = true;

			if (!GetWorldTimerManager().IsTimerActive(SoundResetTimer))
			{
				float DelayTime = 3.f; // Adjust the delay time as needed
				GetWorldTimerManager().SetTimer(SoundResetTimer, this, &AEnemy::ResetSound, DelayTime);
			}
		}

		if (GetCharacterMovement() && GetCharacterMovement()->Velocity.Size() > 0.0f)
		{
			FVector VelocityDirection = GetCharacterMovement()->Velocity.GetSafeNormal(); //Move Direction
			FRotator TargetRotation = VelocityDirection.Rotation();

			FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationInterpSpeed);

			FaceRotation(NewRotation, DeltaTime);
		}

		MyCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (MyCharacter && !MyCharacter->bCharacterDead)
		{
			Distance = GetActorLocation() - MyCharacter->GetActorLocation();

			DistanceInFloat = Distance.Size();

			//UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceInFloat);   

			if (DistanceInFloat <= 200.f)
			{
				bIsAttacking = true;

				AnimInstance = GetMesh()->GetAnimInstance();
				if (AnimInstance && AttackMontage)
				{
					if (!AnimInstance->Montage_IsPlaying(AttackMontage))
					{
						AnimInstance->Montage_Play(AttackMontage);
						int32 Selection = FMath::RandRange(0, 1);
						FName SectionName = FName();

						switch (Selection)
						{
						case 0:
							SectionName = FName("Overhand");
							break;
						case 1:
							SectionName = FName("Hook");
							break;
						default:
							break;
						}
						AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
					}
				}
				GetWorld()->GetTimerManager().ClearTimer(StopAttackHandler);
			}
			else
			{
				if (!GetWorldTimerManager().IsTimerActive(StopAttackHandler))
				{
					float DelayTime = 2.f; // Adjust the delay time as needed
					GetWorldTimerManager().SetTimer(StopAttackHandler, this, &AEnemy::StopAttacking, DelayTime);
				}
			}
		}
	}
}

void AEnemy::ResetSound()
{
	bSoundPlaying = false;
}

void AEnemy::StopAttacking()
{
	bIsAttacking = false;
}

void AEnemy::EnableCollision() 
{ 
	RightHandComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 
	LeftHandComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  
	bCollisionOn = true;
}

void AEnemy::DisableCollision() 
{
	RightHandComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	LeftHandComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	bCollisionOn = false;
}

void AEnemy::SetHasDamaged(bool BoolValue)
{
	bHasDamaged = BoolValue; 
}

void AEnemy::DestroyDeadEnemy()
{
	Destroy();
}

void AEnemy::OnRightHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(AMyCharacter::StaticClass()) && !bHasDamaged && bCollisionOn)
	{
		EnemyDealDamage(EnemyDamage);
		bHasDamaged = true; 
		//UE_LOG(LogTemp, Warning, TEXT("Right hand overlapped with MyCharacter"));
	}
}

void AEnemy::OnLeftHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(AMyCharacter::StaticClass()) && !bHasDamaged && bCollisionOn)
	{
		EnemyDealDamage(EnemyDamage);
		bHasDamaged = true;
		//UE_LOG(LogTemp, Warning, TEXT("Left hand overlapped with MyCharacter"));
	}
}

void AEnemy::ChasePlayer()
{
	EnemyController = Cast<AAIController>(GetController()); 
	if (EnemyController && MyCharacter) 
	{
		EnemyController->MoveToActor(MyCharacter, 10.f); 
	}
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
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore); 

	GetMesh()->SetSimulatePhysics(true);

	// Apply impulse only to the specified bone
	//GetMesh()->AddImpulse(ImpulseDirection * 1000, HitBone, true); 

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

void AEnemy::EnemyDealDamage(float DamageValue) 
{
	if (MyCharacter)
	{
		MyCharacter->CurrentHealth -= DamageValue; 
		MyCharacter->PlayHitReaction();
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

