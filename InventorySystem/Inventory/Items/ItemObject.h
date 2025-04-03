#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemObject.generated.h"

class UImage;
/**
 *
 */
UCLASS(BlueprintType)
class  UItemObject : public UObject
{
	GENERATED_BODY()

public:
	UItemObject();

	UFUNCTION(BlueprintCallable)
	void InitializeItemObject(int32 dimensionX, int32 dimensionY, class UMaterialInterface* InIcon, UMaterialInterface* InRIcon, TSubclassOf<class AItemActor> itemClass);

	FIntPoint GetDimensions() const;

	UMaterialInterface* GetItemImage() const;
	UMaterialInterface* GetIcon() const { return Icon; };
	UMaterialInterface* GetRIcon() const { return RIcon; };
	TSubclassOf<AItemActor> GetItemClass() const { return ItemActorClass; }

	void Rotate() { bRotated = !bRotated; }
	bool IsRotated() const { return bRotated; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInterface* Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInterface* RIcon;
	bool bRotated;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIntPoint Dimensions;

	UPROPERTY()
	TSubclassOf<AItemActor> ItemActorClass;
};