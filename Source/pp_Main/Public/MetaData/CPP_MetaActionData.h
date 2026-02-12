#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMetaData.h"
#include "CPP_MetaActionData.generated.h"


UENUM(BlueprintType)
enum class E_ACTIONTYPE : uint8
{
	Evasion			UMETA(DisplayName = "Evasion"),
	Combo			UMETA(DisplayName = "Combo"),
	Skill			UMETA(DisplayName = "Skill")
};

UCLASS()
class UCPP_MetaActionData : public UAnimMetaData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeatData")
	E_ACTIONTYPE e_ActionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeatData")
	bool m_isCancelCombo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeatData")
	UAnimMontage* m_ComboMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeatData")
	float m_Damage;
};
