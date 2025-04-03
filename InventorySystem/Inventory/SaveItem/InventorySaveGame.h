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

    // Ĭ��ͼ�꣨δ��תʱ��ʾ��
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
    UMaterialInterface* ItemIcon;

    // ��������ת���ͼ��
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

