#include "InputSettingsWidget.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include"Kismet/KismetSystemLibrary.h"

void UInputSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ScrollBox->ClearChildren();
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				InputUserSettings = Subsystem->GetUserSettings();
			}
		}
	}
    if (InputUserSettings && KeySettingsWidgetClass)
    {
        TMap<FName, FKeyMappingRow> MappingRows = InputUserSettings->GetCurrentKeyProfile()->GetPlayerMappingRows();

        for (const auto& MapEntry : MappingRows)
        {
            const FKeyMappingRow& KeyRow = MapEntry.Value;

            for (const FPlayerKeyMapping& PlayerMapping : KeyRow.Mappings)
            {
                if (PlayerMapping.GetSlot() == EPlayerMappableKeySlot::First)
                {
                    FLatentActionInfo LatentInfo;
                    UKismetSystemLibrary::Delay(GetWorld(),0.1f,LatentInfo);
                    // 创建控件实例
                    if (UKeySettingsWidget* KeyWidget = CreateWidget<UKeySettingsWidget>(this, KeySettingsWidgetClass))
                    {
                        // 传递参数
                        KeyWidget->InputUserSettings = InputUserSettings;
                        KeyWidget->KeyMapping = PlayerMapping;

                        // 添加到滚动框
                        ScrollBox->AddChild(KeyWidget);

                        UE_LOG(LogTemp, Log, TEXT("Created widget for: %s"),
                            *PlayerMapping.GetMappingName().ToString());
                    }
                    break; // 每行只创建一个控件
                }
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Missing KeySettingsWidgetClass or InputUserSettings!"));
    }
}

