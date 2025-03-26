#include "KeySettingsWidget.h"
#include "Components/InputKeySelector.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void UKeySettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateInputKeySeletorbyKeyMapping(KeyMapping.GetCurrentKey());
	if (InputKeySelector)
	{
		InputKeySelector->OnKeySelected.AddDynamic(this, &UKeySettingsWidget::OnKeySelected);
		InputKeySelector->OnIsSelectingKeyChanged.AddDynamic(this, &UKeySettingsWidget::OnIsSelectingKeyChanged);
	}

	if (Button_Reset)
	{
		Button_Reset->OnClicked.AddDynamic(this, &UKeySettingsWidget::OnResetClicked);
	}
}

void UKeySettingsWidget::UpdateInputKeySeletorbyKeyMapping(FKey Key)
{
	InputKeySelector->SetSelectedKey(Key);
}

void UKeySettingsWidget::OnKeySelected(FInputChord SelectedKey)
{
	if (bIsKeyChanged)
	{
		bIsKeyChanged = false;
		FKey NewKey = SelectedKey.Key;
		FMapPlayerKeyArgs InArgs;
		InArgs.MappingName = KeyMapping.GetMappingName();
		InArgs.NewKey = NewKey;
		FGameplayTagContainer FailureReason;
		InputUserSettings->MapPlayerKey(InArgs, FailureReason);
		InputUserSettings->ApplySettings();
		InputUserSettings->SaveSettings();
	}

}

void UKeySettingsWidget::OnIsSelectingKeyChanged()
{
	bIsKeyChanged = true;
}

void UKeySettingsWidget::OnResetClicked()
{
	FMapPlayerKeyArgs InArgs;
	InArgs.MappingName = KeyMapping.GetMappingName();
	InArgs.NewKey = KeyMapping.GetDefaultKey();
	FGameplayTagContainer FailureReason;
	InputUserSettings->MapPlayerKey(InArgs,FailureReason);
	InputUserSettings->ApplySettings();
	InputUserSettings->SaveSettings();
	UpdateInputKeySeletorbyKeyMapping(KeyMapping.GetDefaultKey());
}