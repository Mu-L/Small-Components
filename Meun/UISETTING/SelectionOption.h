#pragma once

#include"SelectionOption.generated.h"
USTRUCT(BlueprintType)
struct FSelectionOption
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Option")
	FText Label;
};