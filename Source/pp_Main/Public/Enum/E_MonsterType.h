#pragma once

#include "CoreMinimal.h"
#include "E_MonsterType.generated.h"

UENUM(BlueprintType)
enum class E_MonsterType : uint8
{
	E_NONE        UMETA(DisplayName = "None"),
	E_MELEE         UMETA(DisplayName = "Melee"),
	E_RANGED      UMETA(DisplayName = "Ranged"),
	E_BOSS       UMETA(DisplayName = "Boss"),

};