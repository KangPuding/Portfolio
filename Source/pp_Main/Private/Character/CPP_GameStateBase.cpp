#include "Character/CPP_GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CPP_MainCharacter.h"

ACPP_GameStateBase::ACPP_GameStateBase()
{
}

void ACPP_GameStateBase::BeginPlay()
{
	Super::BeginPlay();

    m_item_Inventory.SetNum(ItemInventorySize);
    m_Equip_Inventory.SetNum(EquipInventorySize);

    UClass* UseClass = SlotClass ? *SlotClass : UCPP_Slot::StaticClass();

    for (int32 i = 0; i < ItemInventorySize; ++i)
    {
        UCPP_Slot* pItemSlot = NewObject<UCPP_Slot>(this, UseClass);

        if (pItemSlot)
        {
            pItemSlot->SetItemClass(nullptr);
            pItemSlot->SetItemCount(0);
        }
        m_item_Inventory[i] = pItemSlot;
    }

    for (int32 i = 0; i < EquipInventorySize; ++i)
    {
        UCPP_Slot* pEquipSlot = NewObject<UCPP_Slot>(this, UseClass);

        if (pEquipSlot)
        {
            pEquipSlot->SetItemClass(nullptr);
            pEquipSlot->SetItemCount(0);
        }
        m_Equip_Inventory[i] = pEquipSlot;
    }
}

UCPP_Slot* ACPP_GameStateBase::getItmeInventorySlot(int32 nIndex)
{
	if (m_item_Inventory.IsValidIndex(nIndex))
	{
		return m_item_Inventory[nIndex];
	}

	return nullptr;
}

UCPP_Slot* ACPP_GameStateBase::getEquipInventorySlot(int32 nIndex)
{

	if (m_Equip_Inventory.IsValidIndex(nIndex))
	{
		return m_Equip_Inventory[nIndex];
	}
	return nullptr;
}

bool ACPP_GameStateBase::AddToInventory(TSubclassOf<ACPP_Item_Base> ItemClass, int32 Count, bool bIsEquipment, int32& OutIndex)
{
    OutIndex = INDEX_NONE;

    if (ItemClass == nullptr)
        return false;

    TArray<UCPP_Slot*>* pInv = nullptr;

    if (bIsEquipment)
        pInv = &m_Equip_Inventory;
    else
        pInv = &m_item_Inventory;
    
    if (pInv == nullptr)
        return false;

    int32 nStartIndex = 0;

    if (bIsEquipment)
        nStartIndex = EquipSlotCount;
    else
        nStartIndex = 0;

    for (int32 i = nStartIndex; i < pInv->Num(); i++)
    {
        if ((*pInv)[i] && (*pInv)[i]->IsEmptySlot())
        {
            (*pInv)[i]->SetItemClass(ItemClass);
            (*pInv)[i]->SetItemCount(Count);
            OutIndex = i;
            return true;
        }
    }
    return false;
}

bool ACPP_GameStateBase::SwapSlots(UCPP_Slot* pDst, UCPP_Slot* pSrc)
{
    if (!pDst || !pSrc || pDst == pSrc)
        return false;

#if WITH_EDITOR
    UE_LOG(LogTemp, Warning, TEXT("[SwapSlots] ---------------------------"));
    UE_LOG(LogTemp, Warning, TEXT("Dst: %s | Src: %s"),
        *GetNameSafe(pDst), *GetNameSafe(pSrc));
    UE_LOG(LogTemp, Warning, TEXT("Dst Class: %s | Count: %d"),
        *GetNameSafe(pDst->m_ItemClass), pDst->m_ItemCount);
    UE_LOG(LogTemp, Warning, TEXT("Src Class: %s | Count: %d"),
        *GetNameSafe(pSrc->m_ItemClass), pSrc->m_ItemCount);
#endif

    UCPP_Slot* Temp = NewObject<UCPP_Slot>(this, pDst->GetClass());
    Temp->SetItemClass(pDst->m_ItemClass);
    Temp->SetItemCount(pDst->m_ItemCount);

    pDst->SetItemClass(pSrc->m_ItemClass);
    pDst->SetItemCount(pSrc->m_ItemCount);

    pSrc->SetItemClass(Temp->m_ItemClass);
    pSrc->SetItemCount(Temp->m_ItemCount);

    pDst->OnSlotUpdated.Broadcast();
    pSrc->OnSlotUpdated.Broadcast();

#if WITH_EDITOR
    UE_LOG(LogTemp, Warning, TEXT("[SwapSlots] COMPLETE"));
#endif

    ACPP_MainCharacter* MC = Cast<ACPP_MainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (MC)
    {
        MC->RecalcEquipStats();
    }
    return true;
}
