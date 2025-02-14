
#include "Password/PasswordUserWidget.h"

#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ButtonSlot.h"
#include "Components/UniformGridSlot.h"

// 初始化 Widget
void UPasswordUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 绑定按钮点击事件
    if (ConfirmButton)
    {
        ConfirmButton->OnClicked.AddDynamic(this, &UPasswordUserWidget::OnConfirmButtonClicked);
    }

    if (ClearButton)
    {
        ClearButton->OnClicked.AddDynamic(this, &UPasswordUserWidget::OnClearButtonClicked);
    }
    NumberGridPanel->SetVisibility(ESlateVisibility ::Visible);
}

// 设置密码
void UPasswordUserWidget::SetPassword(const FString& NewPassword)
{
    CorrectPassword = NewPassword;
}

// 验证密码
bool UPasswordUserWidget::ValidatePassword()
{
    FString EnteredPasswordString;
    for (int32 Number : EnteredPassword)
    {
        EnteredPasswordString += FString::FromInt(Number);
    }

    if (EnteredPasswordString == CorrectPassword)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// 数字按钮点击事件
void UPasswordUserWidget::OnNumberButtonClicked(int32 Number)
{ 
        EnteredPassword.Add(Number);
        UpdatePasswordDisplay();
}

// 清除按钮点击事件
void UPasswordUserWidget::OnClearButtonClicked()
{
    EnteredPassword.Empty();
    UpdatePasswordDisplay();
}

// 确定按钮点击事件
void UPasswordUserWidget::OnConfirmButtonClicked()
{
    ValidatePassword();
}

// 更新密码显示
void UPasswordUserWidget::UpdatePasswordDisplay()
{
    FString DisplayText;
    for (int32 Number : EnteredPassword)
    {
        DisplayText += FString::FromInt(Number);
    }

    if (PasswordDisplayText)
    {
        PasswordDisplayText->SetText(FText::FromString(DisplayText));
    }
}


