#pragma once

#include "CoreMinimal.h"
#include "E_WeaponType.generated.h"

// 이넘이랑 무기 인덱스는 일치 시켜야함 
UENUM(BlueprintType)
enum class E_WeaponTypes : uint8
{
	E_NONE        UMETA(DisplayName = "None"),       
	E_BOW         UMETA(DisplayName = "Bow"),        
	E_DAGGER      UMETA(DisplayName = "Dagger"),     
	E_SPEAR       UMETA(DisplayName = "Spear"),      
	E_GREATESWORD  UMETA(DisplayName = "Great Sword"),
	E_KATANA      UMETA(DisplayName = "Katana")      
};