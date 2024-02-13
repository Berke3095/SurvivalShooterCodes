// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "HUD/MyHUD.h"
#include "Math/UnrealMathUtility.h"
#include "Characters/MyCharacter.h"
#include "Enemy/Enemy.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bDrawDebugTrace = false;
}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

//Bullet trace
void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	MyCharacter = Cast<AMyCharacter>(GetOwner());
	MyHud = Cast<AMyHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (MyHud && MyCharacter && !MyCharacter->bCharacterDead)
	{	
		//Getting the random bullet spread 
		if (MyHud->DistanceToCenter > 20.f && MyCharacter->GetFireState() == true)
		{
			BulletSpread = 2.5f;
			RandomPitch = FMath::RandRange(-BulletSpread, BulletSpread);
			RandomYaw = FMath::RandRange(-BulletSpread, BulletSpread);
		}
		else 
		{
			BulletSpread = 0.6f;
			RandomPitch = FMath::RandRange(-BulletSpread, BulletSpread);
			RandomYaw = FMath::RandRange(-BulletSpread, BulletSpread);
		}
	}
	
	//Get screen size
	FVector2d ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	//Center of screen
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	//Center of screen to world
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);
	//Drawing the trace
	if (bScreenToWorld)
	{
		if (MyCharacter->GetAimState() == false) { return; }
		//Setting random world directions for bullet spread 
		CrosshairWorldDirection = FQuat(CrosshairWorldDirection.Rotation() + FRotator(RandomPitch, RandomYaw, 0.0f)).Vector(); 
		//Trace line
		TraceStart = CrosshairWorldPosition;
		if (MyCharacter)
		{
			float DistanceToCharacter = (MyCharacter->GetActorLocation() - TraceStart).Size();
			TraceStart += CrosshairWorldDirection * (DistanceToCharacter + 20.f);
		}

		TraceEnd = TraceStart + CrosshairWorldDirection * TRACE_LENGTH; 

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			TraceStart,
			TraceEnd,
			ECollisionChannel::ECC_Visibility
			);
		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = TraceEnd;
			HitTarget = TraceEnd; //Hit target location definition for fire function
		}
		else
		{
			HitTarget = TraceHitResult.ImpactPoint; 
			//Drawing sphere at the hitpoint
			if (bDrawDebugTrace)
			{
				DrawDebugSphere(
					GetWorld(),
					TraceHitResult.ImpactPoint,
					6.f,
					12,
					FColor::Red
				);
			}	
		}
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Shooting mechanics
	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult);

	TArray<AActor*> FoundActors; 

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		AEnemy* Enemy = Cast<AEnemy>(Actor);
		if (HitResult.GetActor() == Enemy)  
		{
			Enemy->bEnemyCanDodge = true; 
		}
	}
}

