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

	UPROPERTY(BlueprintReadOnly)
	class AMyCharacter* MyCharacter;

	ECharacterState CharacterState;

	UFUNCTION(BlueprintCallable)
	void SpawnEnemyAtRandom(TArray<TSubclassOf<AActor>> EnemiesToSpawn);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int32 NumOfEnemy = 0; 

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float EnemySpawnDelay = 1.f;

	FTimerHandle EnemySpawnTimer;

	bool bEnemySpawnTimerActive;

	float MinSpawnRadius = 5000;
	float MaxSpawnRadius = 8000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 MaxNumOfEnemy = 30;
};
