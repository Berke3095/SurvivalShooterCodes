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
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()); 
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CharacterState = MyCharacter->GetCharacterState();

	if (CharacterState == ECharacterState::ECS_EquippedRifle)
	{
		if (!bBasicSpawnTimerActive && NumOfBasics <= 30)
		{
			GetWorld()->GetTimerManager().SetTimer(BasicSpawnTimer, this, &AEnemySpawner::SpawnBasicAtRandom, BasicSpawnDelay); 
			bBasicSpawnTimerActive = true;
			NumOfBasics++;
		}
		if (!bBigMouthSpawnTimerActive && NumOfBigMouth <= 5)
		{
			GetWorld()->GetTimerManager().SetTimer(BigMouthSpawnTimer, this, &AEnemySpawner::SpawnBigMouthAtRandom, BigMouthSpawnDelay); 
			bBigMouthSpawnTimerActive = true;
			NumOfBigMouth++;
		}
		if (!bMutantSpawnTimerActive && NumOfMutant <= 1)
		{
			GetWorld()->GetTimerManager().SetTimer(MutantSpawnTimer, this, &AEnemySpawner::SpawnMutantAtRandom, MutantSpawnDelay); 
			bMutantSpawnTimerActive = true;
			NumOfMutant++;
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("Number of Basics: %d"), NumOfBasics);
	//UE_LOG(LogTemp, Warning, TEXT("Number of BigMouth: %d"), NumOfBigMouth); 
	//UE_LOG(LogTemp, Warning, TEXT("Number of Mutant: %d"), NumOfMutant); 

}

void AEnemySpawner::SpawnEnemyAtRandom(int32 EnemyIndex, bool& bSpawnTimerActive, FTimerHandle& SpawnTimer)
{
	if (EnemiesToSpawn.Num() > 0)
	{
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (NavSys)
		{
			FNavLocation RandomNavLocation;
			FVector MyCharacterLocation = MyCharacter->GetActorLocation();

			bool bValidSpawnPointFound = false;

			while (!bValidSpawnPointFound)
			{
				if (NavSys->GetRandomPointInNavigableRadius(MyCharacterLocation, MaxSpawnRadius, RandomNavLocation))
				{
					float Distance = FVector::Dist(MyCharacter->GetActorLocation(), RandomNavLocation);
					if (Distance >= MinSpawnRadius)
					{
						FRotator Rotation = MyCharacter->GetActorRotation();
						GetWorld()->SpawnActor<AActor>(EnemiesToSpawn[EnemyIndex], RandomNavLocation, Rotation);
						bValidSpawnPointFound = true;
					}
				}
			}
		}
	}

	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
	bSpawnTimerActive = false;
}

void AEnemySpawner::SpawnBasicAtRandom()
{
	SpawnEnemyAtRandom(FMath::RandRange(0, 3), bBasicSpawnTimerActive, BasicSpawnTimer);
}

void AEnemySpawner::SpawnBigMouthAtRandom()
{
	SpawnEnemyAtRandom(4, bBigMouthSpawnTimerActive, BigMouthSpawnTimer);
}

void AEnemySpawner::SpawnMutantAtRandom()
{
	SpawnEnemyAtRandom(5, bMutantSpawnTimerActive, MutantSpawnTimer);
}

