// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/CharacterStates.h"
#include "EnemySpawner.generated.h"

UCLASS()
class GREEKISLAND_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class AMyCharacter* MyCharacter;

	ECharacterState CharacterState;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int32 NumOfBasics = 0; 
	int32 NumOfBigMouth = 0;
	int32 NumOfMutant = 0;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AActor>> EnemiesToSpawn; 

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true")) 
	float BasicSpawnDelay = 1.f;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float BigMouthSpawnDelay = 15.f;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float MutantSpawnDelay = 40.f;

	FTimerHandle BasicSpawnTimer; 
	FTimerHandle BigMouthSpawnTimer;
	FTimerHandle MutantSpawnTimer;

	bool bBasicSpawnTimerActive = false;
	bool bBigMouthSpawnTimerActive = false;
	bool bMutantSpawnTimerActive = false;

	bool bValidSpawnPointFoundBasic;
	bool bValidSpawnPointFoundBigMouth;
	bool bValidSpawnPointFoundMutant;

	void SpawnEnemyAtRandom(int32 EnemyIndex, bool& bSpawnTimerActive, FTimerHandle& SpawnTimer);
	void SpawnBasicAtRandom();
	void SpawnBigMouthAtRandom();  
	void SpawnMutantAtRandom();

	float MinSpawnRadius = 5000;
	float MaxSpawnRadius = 8000;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 MaxNumOfBasic = 30;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 MaxNumOfBigMouth = 5;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 MaxNumOfMutant = 1;

};
