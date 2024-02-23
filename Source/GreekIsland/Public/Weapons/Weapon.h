// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 
#include "Characters/CharacterStates.h"
#include "Weapon.generated.h"

UCLASS()
class GREEKISLAND_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	virtual void BeginPlay() override;

	//Sphere component
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Weapon
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* AreaSphere;

	//Pickup widget
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UWidgetComponent* PickupWidget;

	UPROPERTY(EditAnywhere, Category = "Weapon Effects")
	class UAnimationAsset* FireAnimation;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere) 
	TSubclassOf<class ABulletShell> BulletShellClass;  

	class UCombatComponent* CombatComponent;

	UPROPERTY(EditAnywhere)
	float WeaponRotationSpeed = 50.0f;

public:	

	//Weapon equip function
	FORCEINLINE void Equip(USceneComponent* InParent, FName InSocketName);

	//Pickup widget visibility
	FORCEINLINE void ShowPickupWidget(bool bShowWidget);

	FORCEINLINE void Fire(const FVector& HitTarget);

	FORCEINLINE void SpendAmmo(); 

	UPROPERTY(EditAnywhere) 
	int32 CurrentAmmo; 

	UPROPERTY(EditAnywhere)
	int32 MaxAmmoClip; 

	EWeaponState WeaponState = EWeaponState::EWS_UnEquipped; 

	virtual void Tick(float DeltaTime) override;
};
