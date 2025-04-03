#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item.generated.h"

class UItemObject;
class UImage;
class UBorder;
class USizeBox;
/**
 *
 */

UCLASS()
class  UItem : public UUserWidget
{
    GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoved, UItemObject*, ItemObject);
	void RefreshItem();
	void GetItemIcon() const;

	void InitializeItem(UItemObject* InItemObject, float InSize);
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	FOnRemoved OnRemoved;
protected:

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USizeBox* BackgroundSizeBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* BackgroundBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY()
	UItemObject* ItemObject;

	FVector2D Size;
	
private:

	float TileSize = 50.f;
};
