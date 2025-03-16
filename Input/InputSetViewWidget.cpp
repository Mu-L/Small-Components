#include "InputSetViewWidget.h"
#include "InputSetWidget.h"
#include "EnhancedInputSubsystems.h"

void UInputSetViewWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!DefaultContext || !MoveAction)
    {
        UE_LOG(LogTemp, Error, TEXT("[ERROR] Missing default context or move action!"));
        return;
    }

    // 创建共享上下文
    SharedContext = DuplicateObject(DefaultContext, this);
    UE_LOG(LogTemp, Log, TEXT("Created shared context: %s"), *GetNameSafe(SharedContext));

    // 配置方向控件
    TArray<UInputSetWidget*> DirectionWidgets = {
        MoveForward,
        MoveBackward,
        MoveLeft,
        MoveRight
    };

    for (int32 i = 0; i < DirectionWidgets.Num(); ++i)
    {
        if (UInputSetWidget* Widget = DirectionWidgets[i])
        {
            Widget->OriginalContext = SharedContext;
            Widget->LinkedAction = MoveAction;
            Widget->ActionKeyIndex = i;
            Widget->InitializeWidget();
        }
    }

    // 应用初始映射
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            // 先移除旧版上下文（如果存在）
            if (Subsystem->HasMappingContext(SharedContext))
            {
                Subsystem->RemoveMappingContext(SharedContext);
            }

            // 添加新版上下文
            Subsystem->AddMappingContext(SharedContext, 0);
        }
    }

}

