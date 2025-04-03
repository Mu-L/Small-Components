#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include"Items/ItemObject.h"
#include "InventorySaveGame.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 SlotIndex;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
    TSubclassOf<UItemObject> ItemObjectClass;

    // 默认图标（未旋转时显示）
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
    UMaterialInterface* ItemIcon;

    // 新增：旋转后的图标
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
    UMaterialInterface* RotatedItemIcon;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
    bool bRotated;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 dimensionX;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 dimensionY;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
    TSubclassOf<AItemActor> itemClass;
};

UCLASS()
class UInventorySaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UInventorySaveGame();

    // Array to hold the saved inventory data
    UPROPERTY(VisibleAnywhere, Category = "Inventory")
    TArray<FInventorySlot> InventorySlots;
};

