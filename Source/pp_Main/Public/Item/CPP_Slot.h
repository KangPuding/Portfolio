#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CPP_Item_Base.h"
#include "CPP_Slot.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlotUpdated);

UCLASS(BlueprintType, Blueprintable)
class UCPP_Slot : public UObject
{
    GENERATED_BODY()

public:
    UCPP_Slot();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    ACPP_Item_Base* m_ItemRef;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TSubclassOf<ACPP_Item_Base> m_ItemClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 m_ItemCount;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
    TSubclassOf<ACPP_Item_Base> m_DefaultItemClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|UI")
    UTexture2D* m_CachedIcon;

    UPROPERTY(BlueprintAssignable, Category = "Event")
    FOnSlotUpdated OnSlotUpdated;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|UI")
    FText m_CachedDescription;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|UI")
    FText m_CachedExpain;

public:
    UFUNCTION(BlueprintCallable, Category = "Item")
    void SetItemClass(TSubclassOf<ACPP_Item_Base> InClass);

    UFUNCTION(BlueprintCallable, Category = "Item")
    void SetItemCount(int32 Count);

    UFUNCTION(BlueprintPure, Category = "Item")
    bool HasItem() const;

    UFUNCTION(BlueprintPure, Category = "Item")
    bool IsEmptySlot() const;

    UTexture2D* GetDefaultIcon() const;

    UFUNCTION(BlueprintPure, Category = "Item|UI")
    UTexture2D* GetIconCached() const { return m_CachedIcon; }

    UFUNCTION(BlueprintPure, Category = "Item|UI")
    FText GetDescription() const { return m_CachedDescription; }

    UFUNCTION(BlueprintPure, Category = "Item|UI")
    FText GetExpain() const { return m_CachedExpain; }
};
