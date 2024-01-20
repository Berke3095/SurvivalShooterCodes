#pragma once

//Character states
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Equipped UMETA(DisplayName = "Equipped"),
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
};