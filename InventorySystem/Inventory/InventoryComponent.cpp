#include "InventoryComponent.h"
#include"InventorySaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "ItemObject.h"
#include "ItemActor.h"



UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Items.Init(nullptr, Rows * Columns);
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsDirty)
	{
		OnInventoryChanged.Broadcast();
		IsDirty = false;
	}
}

void UInventoryComponent::RemoveItem(UItemObject* ItemObject)
{
	if (ItemObject)
	{
		for (int32 i = 0; i < Items.Num(); i++)
		{
			if (Items[i] == ItemObject)
			{
				Items[i] = nullptr;
			}
		}
		IsDirty = true;
	}
}

bool UInventoryComponent::TryAddItem(UItemObject* ItemObject)
{
	if (!ItemObject)
	{
		return false;
	}
	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (IsRoomAvailable(ItemObject, i))
		{
			AddItemAt(ItemObject, i);
			return true;
		}
	}
	for (int32 i = 0; i < Items.Num(); i++)
	{
		ItemObject->Rotate();
		if (IsRoomAvailable(ItemObject, i))
		{
			AddItemAt(ItemObject, i);
			return true;
		}
		ItemObject->Rotate();
	}
	return false;
}

void UInventoryComponent::AddItemAt(UItemObject* ItemObject, int32 TopLeftIndex)
{
	int32 tileX = IndexToTile(TopLeftIndex).X;
	int32 tileY = IndexToTile(TopLeftIndex).Y;
	for (int32 n = tileX; n < tileX + ItemObject->GetDimensions().X; n++)
	{
		for (int32 m = tileY; m < tileY + ItemObject->GetDimensions().Y; m++)
		{
			int32 index = TileToIndex(FVector2D(n, m));
			Items[index] = ItemObject;
		}
	}
	IsDirty = true;
}

bool UInventoryComponent::IsRoomAvailable(UItemObject* ItemObject, int32 TopLeftIndex) const
{

	int32 tileX = IndexToTile(TopLeftIndex).X;
	int32 tileY = IndexToTile(TopLeftIndex).Y;
	for (int32 n = tileX; n < tileX + ItemObject->GetDimensions().X; n++)
	{
		for (int32 m = tileY; m < tileY + ItemObject->GetDimensions().Y; m++)
		{
			if (n >= 0 && m >= 0 && n < Columns && m < Rows)
			{
				int32 index = TileToIndex(FVector2D(n, m));
				if (!Items.IsValidIndex(index))
				{
					return false;
				}
				if (Items[index] != nullptr)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

TMap<UItemObject*, FVector2D> UInventoryComponent::GetAllItems() const
{
	TMap<UItemObject*, FVector2D> ItemsMap;

	for (int i = 0; i < Items.Num(); i++)
	{
		if (Items[i] != nullptr && !ItemsMap.Contains(Items[i]))
		{
			ItemsMap.Add(Items[i], IndexToTile(i));
		}
	}
	return ItemsMap;
}

FVector2D UInventoryComponent::IndexToTile(int32 Index) const
{
	return FVector2D(Index % Columns, Index / Columns);
}

int32 UInventoryComponent::TileToIndex(FVector2D Tile) const
{
	return Tile.X + Tile.Y * Columns;
}

void UInventoryComponent::SaveInventory()
{
	UInventorySaveGame* SaveGameInstance = Cast<UInventorySaveGame>(UGameplayStatics::CreateSaveGameObject(UInventorySaveGame::StaticClass()));

	TArray<FInventorySlot> SavedSlots;
	int SlotIndex = 0;
	for (auto& ItemPair : GetAllItems())
	{
		UItemObject* ItemObject = ItemPair.Key;
		if (ItemObject)
		{
			FInventorySlot Slot;
			Slot.SlotIndex = SlotIndex++;

			// �ؼ��޸ģ�����ԭʼ�ߴ磨��תǰ�ĳߴ磩
			FIntPoint OriginalDimensions = ItemObject->IsRotated() ?
				FIntPoint(ItemObject->GetDimensions().Y, ItemObject->GetDimensions().X) :
				ItemObject->GetDimensions();

			Slot.ItemObjectClass = ItemObject->GetClass();
			Slot.itemClass = ItemObject->GetItemClass();
			Slot.dimensionX = OriginalDimensions.X;
			Slot.dimensionY = OriginalDimensions.Y;

		
			Slot.bRotated = ItemObject->IsRotated();
			Slot.ItemIcon = ItemObject->GetIcon();          // Ĭ��ͼ��
			Slot.RotatedItemIcon = ItemObject->GetRIcon();  // ��ת���ͼ��
			
			SavedSlots.Add(Slot);
		}
	}

	SaveGameInstance->InventorySlots = SavedSlots;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, "InventorySlot", 0);
}
void UInventoryComponent::LoadInventory()
{
	if (USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot("InventorySlot", 0))
	{
		UInventorySaveGame* SaveGameInstance = Cast<UInventorySaveGame>(LoadedGame);

		if (SaveGameInstance)
		{
			for (const FInventorySlot& Slot : SaveGameInstance->InventorySlots)
			{
			
				UItemObject* ItemObject = NewObject<UItemObject>(this, Slot.ItemObjectClass);

				// �ؼ��޸ģ�������ת״̬ѡ��ͼ��
				UMaterialInterface* DefaultIcon = Slot.ItemIcon;
				UMaterialInterface* RotatedIcon = Slot.RotatedItemIcon;

				ItemObject->InitializeItemObject(
					Slot.dimensionX,
					Slot.dimensionY,
					DefaultIcon,     // Ĭ��ͼ��
					RotatedIcon,     // ��ת���ͼ��
					Slot.itemClass
				);

				if (Slot.bRotated)
				{
					ItemObject->Rotate(); // ����Զ��л���ʾ��ͼ��
				}

				TryAddItem(ItemObject);
		
			}
		}
	}
}