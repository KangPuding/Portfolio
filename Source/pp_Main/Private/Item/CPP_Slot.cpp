#include "Item/CPP_Slot.h"
#include "Item/CPP_Item_Base.h"
#include "Engine/Engine.h"

UCPP_Slot::UCPP_Slot()
{
    m_ItemClass = nullptr;
    m_ItemCount = 0;
    m_CachedIcon = nullptr;
}

void UCPP_Slot::SetItemClass(TSubclassOf<ACPP_Item_Base> InClass)
{
    if (m_ItemClass == InClass)
        return;

    m_ItemClass = InClass;
    m_CachedIcon = nullptr;
    m_CachedDescription = FText::GetEmpty();
    m_CachedExpain = FText::GetEmpty();

    if (!m_ItemClass)
    {
        if (m_ItemCount > 0)
        {
            OnSlotUpdated.Broadcast();
            return;
        }

        m_CachedIcon = GetDefaultIcon();
        OnSlotUpdated.Broadcast();
        return;
    }

    if (m_ItemClass->IsChildOf(ACPP_Item_Base::StaticClass()))
    {
        if (const ACPP_Item_Base* CDO = m_ItemClass->GetDefaultObject<ACPP_Item_Base>())
        {
            m_CachedIcon = CDO->GetIcon();
            m_CachedDescription = CDO->GetDescription();
            m_CachedExpain = CDO->GetExpain();
        }
    }
    else
    {
        m_ItemClass = nullptr;
        m_CachedIcon = GetDefaultIcon();
        m_CachedDescription = GetDescription();
        m_CachedExpain = GetExpain();
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("[Slot] SetItemClass INVALID TYPE, fallback to default"));
#endif
        OnSlotUpdated.Broadcast();
        return;
    }
    if (!m_CachedIcon)
        m_CachedIcon = GetDefaultIcon();

#if WITH_EDITOR
    if (GEngine)
    {
        const FString Name = m_ItemClass ? m_ItemClass->GetName() : TEXT("None");
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow,
            FString::Printf(TEXT("[Slot] Class Set ¡æ %s | CachedIcon=%s"),
                *Name, m_CachedIcon ? *m_CachedIcon->GetName() : TEXT("None")));
    }
#endif

    OnSlotUpdated.Broadcast();
}

void UCPP_Slot::SetItemCount(int32 Count)
{
    m_ItemCount = Count;
    OnSlotUpdated.Broadcast();
}

bool UCPP_Slot::HasItem() const
{
    return (m_ItemClass && m_ItemClass != m_DefaultItemClass && m_ItemCount > 0);
}

bool UCPP_Slot::IsEmptySlot() const
{
    return (!m_ItemClass || m_ItemClass == m_DefaultItemClass || m_ItemCount <= 0);
}

UTexture2D* UCPP_Slot::GetDefaultIcon() const
{
    if (m_DefaultItemClass && m_DefaultItemClass->IsChildOf(ACPP_Item_Base::StaticClass()))
    {
        if (const ACPP_Item_Base* CDO = m_DefaultItemClass->GetDefaultObject<ACPP_Item_Base>())
        {
            return CDO->GetIcon();
        }
    }
    return nullptr;
}
