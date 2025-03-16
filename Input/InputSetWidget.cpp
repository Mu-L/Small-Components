#include "InputSetWidget.h"
#include "Components/InputKeySelector.h"
#include "EnhancedInputSubsystems.h"
#include"EnhancedInputLibrary.h"
#include"InputMappingContext.h"
#include "GameFramework/PlayerController.h"

// InputSetWidget.cpp

void UInputSetWidget::NativeConstruct()
{
    Super::NativeConstruct();
    InitializeWidget();
}

void UInputSetWidget::InitializeWidget()
{
    UE_LOG(LogTemp, Warning, TEXT("[InitializeWidget] Initializing %s"), *GetName());

    // 验证必要组件
    if (!OriginalContext || !LinkedAction || !KeySelector)
    {
        UE_LOG(LogTemp, Error, TEXT("[ERROR] Missing essential components in %s"), *GetName());
        return;
    }

    // 重置状态
    TargetMappingIndex = -1;
    OriginalModifiers.Empty();
    OriginalTriggers.Empty();

    // 查找目标映射
    int32 FoundCount = 0;
    const TArray<FEnhancedActionKeyMapping>& Mappings = OriginalContext->GetMappings();

    for (int32 i = 0; i < Mappings.Num(); ++i)
    {
        const FEnhancedActionKeyMapping& Mapping = Mappings[i];
        if (Mapping.Action == LinkedAction)
        {
            if (FoundCount == ActionKeyIndex)
            {
                TargetMappingIndex = i;
                OriginalKey = Mapping.Key;

                // 深拷贝修饰符和触发器
                OriginalModifiers.Append(Mapping.Modifiers);
                OriginalTriggers.Append(Mapping.Triggers);

                KeySelector->SetSelectedKey(OriginalKey);
                UE_LOG(LogTemp, Log, TEXT("Found target mapping at index %d"), i);
                break;
            }
            FoundCount++;
        }
    }

    if (TargetMappingIndex == -1)
    {
        UE_LOG(LogTemp, Error, TEXT("[ERROR] Failed to find mapping for action %s at index %d"),
            *GetNameSafe(LinkedAction), ActionKeyIndex);
        return;
    }

    // 绑定事件
    KeySelector->OnKeySelected.AddDynamic(this, &UInputSetWidget::OnKeySelected);
}

void UInputSetWidget::OnKeySelected(FInputChord SelectedKey)
{
    if (!OriginalContext || TargetMappingIndex == -1)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ignoring key selection - invalid state"));
        return;
    }

    // 获取当前映射键
    const FKey OldKey = OriginalKey;

    // 解除旧键绑定
    OriginalContext->UnmapKey(LinkedAction, OldKey);

    // 添加新键绑定（保留原有配置）
    FEnhancedActionKeyMapping& NewMapping = OriginalContext->MapKey(
        LinkedAction,
        SelectedKey.Key
    );

    // 恢复修饰符和触发器
    NewMapping.Modifiers = OriginalModifiers;
    NewMapping.Triggers = OriginalTriggers;

    // 更新原始键记录
    OriginalKey = SelectedKey.Key;

    // 使用官方推荐方式重建输入
    UEnhancedInputLibrary::RequestRebuildControlMappingsUsingContext(OriginalContext, true);

    UE_LOG(LogTemp, Log, TEXT("Updated mapping: %s -> %s"),
        *OldKey.ToString(),
        *SelectedKey.Key.ToString());
}

