#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Enum/E_EquipType.h"
#include "CPP_Item_Base.generated.h"

UCLASS()
class ACPP_Item_Base : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Highlight")
	UMaterialInterface* m_OverlayMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Texture")
	UTexture2D* m_icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|UI", meta = (MultiLine = "true"))
	FText m_Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|UI", meta = (MultiLine = "true"))
	FText m_Expain;


public:
	ACPP_Item_Base();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void HighLight(bool Enable);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool m_isItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool m_isEquip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool m_isUse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Equip")
	E_EquipType EquipType = E_EquipType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Equip")
	int32 EquipSlotIndexOverride = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|BonusStat")
	float BonusMaxHp = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|BonusStat")
	float BonusMaxMp = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|BonusStat")
	float BonusMaxSm = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|BonusStat")
	float BonusDefense = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Affact")
	TSubclassOf<UObject> m_AffactBaseClass;

public:
	UFUNCTION(BlueprintPure, Category = "Item|UI")
	UTexture2D* GetIcon() const { return m_icon; }

	UFUNCTION(BlueprintPure, Category = "Item|UI")
	FText GetDescription() const { return m_Description; }

	UFUNCTION(BlueprintPure, Category = "Item|UI")
	FText GetExpain() const { return m_Expain; }

	UFUNCTION(BlueprintPure, Category = "Item|BonusStat")
	float GetBonusMaxHp() const { return BonusMaxHp; }

	UFUNCTION(BlueprintPure, Category = "Item|BonusStat")
	float GetBonusMaxMp() const { return BonusMaxMp; }

	UFUNCTION(BlueprintPure, Category = "Item|BonusStat")
	float GetBonusMaxSm() const { return BonusMaxSm; }

	UFUNCTION(BlueprintPure, Category = "Item|BonusStat")
	float GetBonusDefense() const { return BonusDefense; }
};
