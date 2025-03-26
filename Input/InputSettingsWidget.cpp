#include "InputSettingsWidget.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
    if (InputUserSettings)
    {
        // 加载控件类（注意需要添加_C后缀）
        static TSubclassOf<UKeySettingsWidget> WidgetClass = LoadClass<UKeySettingsWidget>(
            nullptr,
            TEXT("/Game/HUD/SettingMeun/Keyboard/WBP_KeySettings_Test.WBP_KeySettings_Test_C")
        );

        TMap<FName, FKeyMappingRow> MappingRows = InputUserSettings->GetCurrentKeyProfile()->GetPlayerMappingRows();

        for (const auto& MapEntry : MappingRows)
        {
            const FKeyMappingRow& KeyRow = MapEntry.Value;

            for (const FPlayerKeyMapping& PlayerMapping : KeyRow.Mappings)
            {
                if (PlayerMapping.GetSlot() == EPlayerMappableKeySlot::First)
                {
                    // 创建控件实例
                    if (UKeySettingsWidget* KeyWidget = CreateWidget<UKeySettingsWidget>(this, WidgetClass))
                    {
                        // 设置ExposeOnSpawn参数
                        KeyWidget->InputUserSettings = InputUserSettings;
                        KeyWidget->KeyMapping = PlayerMapping;

                        // 添加到ScrollBox
                        ScrollBox->AddChild(KeyWidget);

                        UE_LOG(LogTemp, Warning, TEXT("Created widget for mapping: %s"),
                            *PlayerMapping.GetMappingName().ToString());
                    }
                    break;
                }
            }
        }
    }
}

