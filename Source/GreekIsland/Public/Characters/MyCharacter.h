// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Characters/CharacterStates.h"
#include "MyCharacter.generated.h"

//Input maps
class UInputAction;
class UInputActionValue;

UCLASS()
class GREEKISLAND_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	virtual void BeginPlay() override;

	//Default character state   
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	//Anim Instance
	class UMyCharacterAnimInstance* MyCharacterAnimInstance; 

	UPROPERTY(EditAnywhere)
	class UAnimMontage* GetHitMontage;

	//Inputs and actions:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* CharacterMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* FireAction;

	//Input functions
	void Move(const FInputActionValue& InputValue);

	void Look(const FInputActionValue& InputValue);

	void Jump(const FInputActionValue& InputValue);

	void Sprint(const FInputActionValue& InputValue);

	void Interact(const FInputActionValue& InputValue);

	void Aim(const FInputActionValue& InputValue);

	void Fire(const FInputActionValue& InputValue);

	//Character direction while aim
	void InterpActorRotation(float DeltaTime);

	//Zoom to aim view
	void InterpFOV(float DeltaTime);

	float DefaultFOV;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = AimSettings)  
	float ZoomedFOV = 55.f;

	UPROPERTY(EditAnywhere, Category = AimSettings)  
	float ZoomInterpSpeed = 20.f; 

	//Aim camera settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AimSettings)
	float AimingArmLength = 80.0f;

	float DefaultArmLength;
	float CurrentArmLength;
	float DefaultSocketOffsetY;
	float CurrentSocketOffsetY;
	float DefaultSocketOffsetZ;
	float CurrentSocketOffsetZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AimSettings) 
	float ZoomSocketOffsetY = 55.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AimSettings)  
	float ZoomSocketOffsetZ = 20.f;

	//No jump repeat
	void ResetJump();
	bool CanJump() const;
	FTimerHandle JumpTimerHandle; 

	//Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArm;

	//Hair
	UPROPERTY(VisibleAnywhere, Category = Hair)
	class UGroomComponent* Groom; 

	//Weapon
	UPROPERTY(VisibleAnywhere)
	class AWeapon* OverlappingWeapon;

	//Bool for firing state
	bool bFiring;

	//Fire montage
	UPROPERTY(EditAnywhere, Category = Montages) 
	UAnimMontage* FireMontage;
	void PlayFireMontage();

	//CombatMechanics component
	class UCombatComponent* CombatComponent;

	//Camera reset after death
	void ResetCamera();

	class UMyOverlay* MyOverlay;

	TArray<AActor*> FoundEnemies; 
	int32 KillCount = 0; 

	FTimerHandle StaminaRegenTimerHandle;
	void Exhaust();

public:	

	FORCEINLINE void SetOverlappingWeapon(AWeapon* Weapon) { OverlappingWeapon = Weapon; }

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	FORCEINLINE bool GetAimState() const { return bAiming; }

	FORCEINLINE bool GetFireState() const { return bFiring; }

	FORCEINLINE float GetMaxWalkSpeed() const;

	FORCEINLINE void PlayHitReaction(); 

	FORCEINLINE void SetKillCount(); 

	//Health attributes
	const float MaxHealth = 100;
	float CurrentHealth;
	bool bCharacterDead = false;

	//Stamina attributes
	const float MaxStamina = 100;
	const float MinStamina = 0.f;
	float CurrentStamina;
	const float SprintStaminaConsumptionRate = 10.f;
	const float SprintStaminaRegenRate = 40.f;
	bool bSprinting = false;
	bool bStaminaZero = false;

	//Bool for aim state
	bool bAiming = false; 

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
