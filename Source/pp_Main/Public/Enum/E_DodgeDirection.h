#pragma once

#include "CoreMinimal.h"
#include "E_DodgeDirection.generated.h"

UENUM(BlueprintType)
enum class E_DodgeDirection : uint8
{
    E_NONE      UMETA(DisplayName = "None"),
    E_FORWARD     UMETA(DisplayName = "Forward"),
    E_BACKWARD    UMETA(DisplayName = "Backward"),
    E_LEFT        UMETA(DisplayName = "Left"),
    E_RIGHT       UMETA(DisplayName = "Right")
};