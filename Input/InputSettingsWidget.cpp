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
        // ���ؿؼ��ࣨע����Ҫ���_C��׺��
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
                    // �����ؼ�ʵ��
                    if (UKeySettingsWidget* KeyWidget = CreateWidget<UKeySettingsWidget>(this, WidgetClass))
                    {
                        // ����ExposeOnSpawn����
                        KeyWidget->InputUserSettings = InputUserSettings;
                        KeyWidget->KeyMapping = PlayerMapping;

                        // ��ӵ�ScrollBox
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

