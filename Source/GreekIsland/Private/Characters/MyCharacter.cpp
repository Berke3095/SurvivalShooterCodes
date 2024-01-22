// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MyCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Weapons/Weapon.h"
#include "Characters/CharacterStates.h"
#include "Characters/MyCharacterAnimInstance.h"
#include "Combat/CombatComponent.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Camera placement
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent); 
	SpringArm->bUsePawnControlRotation = true; 

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	Camera->SetupAttachment(SpringArm);

	//Character rotation relation with camera
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//Groom placement
	Groom = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Groom->SetupAttachment(GetMesh());

}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Get mapping context:
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterMappingContext, 0);
		}
	}

	//Initialize AnimInstance script
	AnimInstance = Cast<UMyCharacterAnimInstance>(GetMesh()->GetAnimInstance()); 
	
	//Setting defaults
	if (Camera)
	{
		DefaultFOV = Camera->FieldOfView;
		CurrentFOV = DefaultFOV;
	}

	if (SpringArm)
	{
		DefaultArmLength = SpringArm->TargetArmLength;
		CurrentArmLength = DefaultArmLength;

		DefaultSocketOffsetY = SpringArm->SocketOffset.Y;
		CurrentSocketOffsetY = DefaultSocketOffsetY;

		DefaultSocketOffsetZ = SpringArm->SocketOffset.Z;
		CurrentSocketOffsetZ = DefaultSocketOffsetZ;
	}
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InterpFOV(DeltaTime);

	InterpActorRotation(DeltaTime);
}

void AMyCharacter::InterpActorRotation(float DeltaTime)
{
	if (bAiming)
	{
		FRotator TargetActorRotation(0.0f, GetControlRotation().Yaw, 0.0f);
		
		if (GetCharacterMovement()->MaxWalkSpeed == 0.0f)
		{
			//Interpolate to the new rotation
			FRotator InterpolatedRotation = FMath::RInterpTo(GetActorRotation(), TargetActorRotation, DeltaTime, 10.0f);
			SetActorRotation(InterpolatedRotation);
		}
		else{ SetActorRotation(TargetActorRotation); } 
		
	}
}
//Setting up camera FOV while aiming
void AMyCharacter::InterpFOV(float DeltaTime)
{
	if (bAiming)
	{
		//Camera FOV
		CurrentFOV = FMath::FInterpTo(CurrentFOV, ZoomedFOV, DeltaTime, ZoomInterpSpeed);

		//Camera location
		CurrentArmLength = FMath::FInterpTo(CurrentArmLength, AimingArmLength, DeltaTime, ZoomInterpSpeed);

		CurrentSocketOffsetY = FMath::FInterpTo(CurrentSocketOffsetY, ZoomSocketOffsetY, DeltaTime, ZoomInterpSpeed);

		CurrentSocketOffsetZ = FMath::FInterpTo(CurrentSocketOffsetZ, ZoomSocketOffsetZ, DeltaTime, ZoomInterpSpeed);
	}
	else
	{
		//Camera FOV
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);

		//Camera location
		CurrentArmLength = FMath::FInterpTo(CurrentArmLength, DefaultArmLength, DeltaTime, ZoomInterpSpeed);

		CurrentSocketOffsetY = FMath::FInterpTo(CurrentSocketOffsetY, DefaultSocketOffsetY, DeltaTime, ZoomInterpSpeed);

		CurrentSocketOffsetZ = FMath::FInterpTo(CurrentSocketOffsetZ, DefaultSocketOffsetZ, DeltaTime, ZoomInterpSpeed);
	}

	if (Camera)
	{
		//Setting FOV
		Camera->SetFieldOfView(CurrentFOV);
	}

	if (SpringArm)
	{
		//Setting springarm location
		SpringArm->TargetArmLength = CurrentArmLength;
		SpringArm->SocketOffset.Y = CurrentSocketOffsetY;
		SpringArm->SocketOffset.Z = CurrentSocketOffsetZ;
	}
}
//Character actions:
void AMyCharacter::Move(const FInputActionValue& InputValue)
{ 
	const FVector2D CurrentValue = InputValue.Get<FVector2D>();

	if (IsValid(GetController()))
	{
		//ForWard and right directions
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//Adding movement input
		AddMovementInput(ForwardDirection, CurrentValue.Y);
		AddMovementInput(RightDirection, CurrentValue.X);

		GetCharacterMovement()->MaxWalkSpeed = 150.f;
	}
}

void AMyCharacter::Look(const FInputActionValue& InputValue)
{
	//Mouse movement
	const FVector2D CurrentValue = InputValue.Get<FVector2D>();
	if (IsValid(GetController()))
	{
		AddControllerYawInput(CurrentValue.X);
		AddControllerPitchInput(CurrentValue.Y);
	}
}

void AMyCharacter::Jump(const FInputActionValue& InputValue)
{
	if (bAiming == true) { return; }

	if (CanJump()) 
	{
		// Jump function already exists
		ACharacter::Jump(); 
		AnimInstance->SetbJustJumped(true); 

		// Set the timer for the delay
		float DelayTime = 1.2f;  
		GetWorldTimerManager().SetTimer(JumpTimerHandle, this, &AMyCharacter::ResetJump, DelayTime, false);  
	}
}
//Jump delay
void AMyCharacter::ResetJump()
{
	// Reset the timer handle
	JumpTimerHandle.Invalidate();

	//Reset jump
	AnimInstance->SetbJustJumped(false);
}

bool AMyCharacter::CanJump() const
{
	// Check if the timer is still active
	return !JumpTimerHandle.IsValid();
}

void AMyCharacter::Sprint(const FInputActionValue& InputValue)
{
	if (bAiming == true) { return; }
	//Adding bool for sprint action
	const bool Run = InputValue.Get<bool>();
	if (Run)
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
	}
}

void AMyCharacter::Interact(const FInputActionValue& InputValue)
{
	if (CharacterState == ECharacterState::ECS_Equipped) { return; }
	const bool Interact = InputValue.Get<bool>();
	if (OverlappingWeapon && Interact)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
		OverlappingWeapon->ShowPickupWidget(false);
		OverlappingWeapon->SetOwner(this);
		CharacterState = ECharacterState::ECS_Equipped;
	}
}

void AMyCharacter::Aim(const FInputActionValue& InputValue)
{
	if (CharacterState == ECharacterState::ECS_Unequipped || AnimInstance->GetbIsFalling() == true) { return; }
	const bool Aim = InputValue.Get<bool>();
	if (Aim)
	{
		bAiming = true;
	}
	else{ bAiming = false; } 
}

void AMyCharacter::Fire(const FInputActionValue& InputValue)
{
	CombatComponent = FindComponentByClass<UCombatComponent>(); 

	if (!bAiming) { return; }
	const bool Fire = InputValue.Get<bool>(); 
	if (Fire)
	{
		bFiring = true;
		if (bFiring)
		{
			if (AnimInstance && AnimInstance->Montage_IsPlaying(FireMontage)) 
			{
				// Don't play the animation if it's already playing
				return;
			}
			PlayFireMontage();
			if (CombatComponent)
			{
				OverlappingWeapon->Fire(CombatComponent->HitTarget);
			}
		}
	}
	else { bFiring = false; }
}

void AMyCharacter::PlayFireMontage()
{
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(FireMontage);
	}
}
// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyCharacter::Jump); 
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMyCharacter::Sprint);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMyCharacter::Interact);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AMyCharacter::Aim); 
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMyCharacter::Fire);  
	}
}

