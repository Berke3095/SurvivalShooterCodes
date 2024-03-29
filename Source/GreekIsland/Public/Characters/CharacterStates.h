#pragma once

//Character states
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_EquippedRifle UMETA(DisplayName = "EquippedRifle"),
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(DisplayName = "Alive"),
	EDP_Dead0 UMETA(DisplayName = "Dead0"), 
	EDP_Dead1 UMETA(DisplayName = "Dead1"),
	EDP_Dead2 UMETA(DisplayName = "Dead2"),
	EDP_Dead3 UMETA(DisplayName = "Dead3"),
};

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_UnEquipped UMETA(DisplayName = "UnEquipped"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
};