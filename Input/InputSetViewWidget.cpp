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

    // ��������������
    SharedContext = DuplicateObject(DefaultContext, this);
    UE_LOG(LogTemp, Log, TEXT("Created shared context: %s"), *GetNameSafe(SharedContext));

    // ���÷���ؼ�
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

    // Ӧ�ó�ʼӳ��
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            // ���Ƴ��ɰ������ģ�������ڣ�
            if (Subsystem->HasMappingContext(SharedContext))
            {
                Subsystem->RemoveMappingContext(SharedContext);
            }

            // ����°�������
            Subsystem->AddMappingContext(SharedContext, 0);
        }
    }

}

