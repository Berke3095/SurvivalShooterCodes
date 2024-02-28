// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemySpawner.h"
#include "Characters/MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Engine/TimerHandle.h" 

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	MyCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawner::SpawnEnemyAtRandom(TArray<TSubclassOf<AActor>>EnemiesToSpawn)
{
	int32 EnemyIndex = FMath::RandRange(0, EnemiesToSpawn.Num() - 1);

	if (EnemiesToSpawn.IsValidIndex(EnemyIndex))
	{
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (NavSys && MyCharacter)
		{
			FNavLocation RandomNavLocation;
			FVector MyCharacterLocation = MyCharacter->GetActorLocation();

			bool bValidSpawnPointFound = false;

			while (!bValidSpawnPointFound)
			{
				if (NavSys->GetRandomReachablePointInRadius(MyCharacterLocation, MaxSpawnRadius, RandomNavLocation))
				{
					float Distance = FVector::Dist(MyCharacter->GetActorLocation(), RandomNavLocation);
					if (Distance >= MinSpawnRadius)
					{
						// Generate a random value between 0 and 1
						float RandomValue = FMath::FRand();

						// Probabilities
						if (RandomValue <= 0.08f)
						{
							EnemyIndex = 0; // 8% chance for index 0
						}
						else if (RandomValue <= 0.12f)
						{
							EnemyIndex = 1; // 12% chance for index 1
						}
						else
						{
							EnemyIndex = FMath::RandRange(2, 5); // 75% chance for indices 2 to 5(Basics)
						}

						FRotator Rotation = MyCharacter->GetActorRotation();
						FVector EnemyNavLocationWithOffsetZ = FVector(RandomNavLocation) + FVector(0, 0, 100);
						AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(EnemiesToSpawn[EnemyIndex], EnemyNavLocationWithOffsetZ, Rotation); 

						if (SpawnedActor)
						{
							bValidSpawnPointFound = true;
							bEnemySpawnTimerActive = true;
							NumOfEnemy++;
						}
					}
				}
			}
		}
	}
	
	if (bEnemySpawnTimerActive)
	{
		GetWorld()->GetTimerManager().ClearTimer(EnemySpawnTimer);
		bEnemySpawnTimerActive = false;
	}
}

