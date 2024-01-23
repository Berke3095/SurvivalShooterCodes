// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BulletShell.h" 
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/World.h"

// Sets default values
ABulletShell::ABulletShell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletShell")); 
	SetRootComponent(ShellMesh); 
	ShellMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	ShellMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	ShellMesh->SetSimulatePhysics(true);
	ShellMesh->SetEnableGravity(true);
	ShellMesh->SetNotifyRigidBodyCollision(true);
	ShellEjectSpeed = 2.0f;
}

// Called when the game starts or when spawned
void ABulletShell::BeginPlay()
{
	Super::BeginPlay();
	
	RandomTorqueAxis = FVector(FMath::FRand(), FMath::FRand(), FMath::FRand()).GetSafeNormal();
	RandomTorqueStrength = FMath::FRandRange(1000.0f, 3000.0f);

	ShellMesh->OnComponentHit.AddDynamic(this, &ABulletShell::OnHit); 

	ShellMesh->AddImpulse(GetActorForwardVector() * ShellEjectSpeed);
	ShellMesh->AddTorqueInDegrees(RandomTorqueAxis * RandomTorqueStrength);
}

void ABulletShell::DestroyShell()
{
	// Destroy the actor
	Destroy();
}

void ABulletShell::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Bullet hit!"));
	if (!bHasPlayedSound && ShellSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
		bHasPlayedSound = true;
	}
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ABulletShell::DestroyShell, 3.0f, false); 
}

// Called every frame
void ABulletShell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

