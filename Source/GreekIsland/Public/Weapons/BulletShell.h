// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletShell.generated.h" 

UCLASS()
class GREEKISLAND_API ABulletShell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletShell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit); 

private:

	//Bullet shell
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* ShellMesh;

	UPROPERTY(EditAnywhere)
	float ShellEjectSpeed;

	//Torque on the shell
	FVector RandomTorqueAxis;
	float RandomTorqueStrength;

	//Sound
	UPROPERTY(EditAnywhere)
	class USoundCue* ShellSound;

	//Destroy bullet after hit
	FTimerHandle DestroyTimerHandle;
	void DestroyShell();

	//Playing shellsound only once
	bool bHasPlayedSound = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
