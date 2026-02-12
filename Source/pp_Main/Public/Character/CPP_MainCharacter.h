#pragma once

#include "CoreMinimal.h"
#include "CPP_CharacterBase.h"
#include "Enum/E_WeaponType.h"
#include "CPP_MainCharacter.generated.h"

class ACPP_WeaponBase;
class ACPP_Item_Base;

UCLASS(Blueprintable)
class ACPP_MainCharacter : public ACPP_CharacterBase
{
	GENERATED_BODY()

public:
	ACPP_MainCharacter();

public:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* Default;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* Bow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* Dagger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* Spear;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* GreatSword;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* Katana;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	E_WeaponTypes m_CurrentWeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	ACPP_WeaponBase* m_CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bool")
	bool is_Aiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bool")
	bool m_isAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bool")
	bool m_isDrawing;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<ACPP_Item_Base*> m_ItemArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	ACPP_Item_Base* m_SelectItem;

	UFUNCTION(BlueprintCallable)
	int32 ItemSelect();

	void OnWeaponChanged(E_WeaponTypes NewWeapon, ACPP_WeaponBase* CurrentWeapon);
	 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bool")
	bool m_isLockInput;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UIBar")
	float m_MaxHp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UIBar")
	float m_CurrentHp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UIBar")
	float m_MaxMp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UIBar")
	float m_CurrentMp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UIBar")
	float m_MaxSm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UIBar")
	float m_CurrentSm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat")
	float m_Defense = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Base")
	float m_BaseMaxHp = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Base")
	float m_BaseMaxMp = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Base")
	float m_BaseMaxSm = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Base")
	float m_BaseDefense = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat|EquipBonus")
	float m_EquipBonusHp = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat|EquipBonus")
	float m_EquipBonusMp = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat|EquipBonus")
	float m_EquipBonusSm = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat|EquipBonus")
	float m_EquipBonusDef = 0.f;

	UFUNCTION(BlueprintCallable, Category = "Stat|Equip")
	void RecalcEquipStats();

private:
	void BindEquipSlots();

	UFUNCTION()
	void OnEquipSlotUpdated();
};
