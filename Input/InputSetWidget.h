#pragma once

#include "EnhancedInputLibrary.h"
#include "EnhancedPlayerInput.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "CommonUserWidget.h"
#include "InputSetWidget.generated.h"

class UInputKeySelector;
class UInputMappingContext;

UCLASS()
class  UInputSetWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 必须设置的蓝图可编辑属性
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
    TObjectPtr<UInputMappingContext> OriginalContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
    TObjectPtr<UInputAction> LinkedAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
    int32 ActionKeyIndex = 0;

protected:
    // 组件绑定
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UInputKeySelector> KeySelector;

    // 运行时状态
    int32 TargetMappingIndex = -1;
    TArray<TObjectPtr<UInputModifier>> OriginalModifiers;
    TArray<TObjectPtr<UInputTrigger>> OriginalTriggers;
    FKey OriginalKey;

public:
    virtual void NativeConstruct() override;
    void InitializeWidget();


    UFUNCTION()
    void OnKeySelected(FInputChord SelectedKey);
};