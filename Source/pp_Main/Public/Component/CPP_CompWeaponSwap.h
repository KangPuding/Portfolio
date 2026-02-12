#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/CPP_MainCharacter.h"
#include "Enum/E_WeaponType.h"
#include "CPP_CompWeaponSwap.generated.h"

class ACPP_WeaponBase;
class UChildActorComponent;
class UCPP_CompMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UCPP_CompWeaponSwap : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<TSubclassOf<ACPP_WeaponBase>> m_Weapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<E_WeaponTypes> E_WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 m_WeaponIndex;

	UCPP_CompMontage* m_CompMontage;
	ACPP_MainCharacter* m_MainCharacter;

public:
	UCPP_CompWeaponSwap();

	void NextWeapon();
	void PrevWeapon();

protected:
	virtual void BeginPlay() override;

	void ChangeWeapon();
	void Vibility();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
