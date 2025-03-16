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
    // �������õ���ͼ�ɱ༭����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
    TObjectPtr<UInputMappingContext> OriginalContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
    TObjectPtr<UInputAction> LinkedAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
    int32 ActionKeyIndex = 0;

protected:
    // �����
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UInputKeySelector> KeySelector;

    // ����ʱ״̬
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