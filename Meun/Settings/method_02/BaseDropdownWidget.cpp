#include "BaseDropdownWidget.h"

// 初始化自定义选项
void UBaseDropdownWidget::InitializeWithCustomOptions(const TArray<FString>& Options)
{
    SettingsComboBox->ClearOptions();
    for (const FString& Option : Options)
    {
        SettingsComboBox->AddOption(Option);
    }
}


FString UBaseDropdownWidget::GetSelectedOption() const
{
    return SettingsComboBox->GetSelectedOption();
}


void UBaseDropdownWidget::SetSelectedOption(const FString& Option)
{
    SettingsComboBox->SetSelectedOption(Option);
}