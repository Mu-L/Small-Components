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

    // ��֤��Ҫ���
    if (!OriginalContext || !LinkedAction || !KeySelector)
    {
        UE_LOG(LogTemp, Error, TEXT("[ERROR] Missing essential components in %s"), *GetName());
        return;
    }

    // ����״̬
    TargetMappingIndex = -1;
    OriginalModifiers.Empty();
    OriginalTriggers.Empty();

    // ����Ŀ��ӳ��
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

                // ������η��ʹ�����
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

    // ���¼�
    KeySelector->OnKeySelected.AddDynamic(this, &UInputSetWidget::OnKeySelected);
}

void UInputSetWidget::OnKeySelected(FInputChord SelectedKey)
{
    if (!OriginalContext || TargetMappingIndex == -1)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ignoring key selection - invalid state"));
        return;
    }

    // ��ȡ��ǰӳ���
    const FKey OldKey = OriginalKey;

    // ����ɼ���
    OriginalContext->UnmapKey(LinkedAction, OldKey);

    // ����¼��󶨣�����ԭ�����ã�
    FEnhancedActionKeyMapping& NewMapping = OriginalContext->MapKey(
        LinkedAction,
        SelectedKey.Key
    );

    // �ָ����η��ʹ�����
    NewMapping.Modifiers = OriginalModifiers;
    NewMapping.Triggers = OriginalTriggers;

    // ����ԭʼ����¼
    OriginalKey = SelectedKey.Key;

    // ʹ�ùٷ��Ƽ���ʽ�ؽ�����
    UEnhancedInputLibrary::RequestRebuildControlMappingsUsingContext(OriginalContext, true);

    UE_LOG(LogTemp, Log, TEXT("Updated mapping: %s -> %s"),
        *OldKey.ToString(),
        *SelectedKey.Key.ToString());
}

