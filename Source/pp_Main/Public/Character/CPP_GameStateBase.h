#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Item/CPP_Slot.h"
#include "Enum/E_EquipType.h"
#include "CPP_GameStateBase.generated.h"


UCLASS()
class ACPP_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	ACPP_GameStateBase();

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 ItemInventorySize = 72;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 EquipInventorySize = 69;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UCPP_Slot> SlotClass = UCPP_Slot::StaticClass();

	// 아이템 전용 인벤토리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<UCPP_Slot*> m_item_Inventory;

	//장비 전용 인벤토리 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip")
	TArray<UCPP_Slot*> m_Equip_Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 EquipSlotCount = 5;

public:
	UFUNCTION(BlueprintCallable)
	UCPP_Slot* getItmeInventorySlot(int32 nIndex);

	UFUNCTION(BlueprintCallable)
	UCPP_Slot* getEquipInventorySlot(int32 nIndex);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddToInventory(TSubclassOf<ACPP_Item_Base> ItemClass, int32 Count, bool bIsEquipment, int32& OutIndex);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool SwapSlots(UCPP_Slot* pDst, UCPP_Slot* pSrc);

};
