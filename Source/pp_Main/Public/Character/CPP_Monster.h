#pragma once

#include "CoreMinimal.h"
#include "Character/CPP_CharacterBase.h"
#include "Enum/E_MonsterType.h"
#include "CPP_Monster.generated.h"

UCLASS(Blueprintable)
class PP_MAIN_API ACPP_Monster : public ACPP_CharacterBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	TArray<E_MonsterType> m_Monster;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	E_MonsterType E_MonsterType;
};
