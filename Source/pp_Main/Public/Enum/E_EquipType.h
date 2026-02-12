#pragma once

#include "CoreMinimal.h"
#include "E_EquipType.generated.h"

UENUM(BlueprintType)
enum class E_EquipType : uint8
{
    None        UMETA(DisplayName = "None"),
    Helmet      UMETA(DisplayName = "Helmet"),
    Chest       UMETA(DisplayName = "Chest"),
    Belts       UMETA(DisplayName = "Belts"),
    Shoes       UMETA(DisplayName = "Shoes"),
    Gloves      UMETA(DisplayName = "Gloves")
};