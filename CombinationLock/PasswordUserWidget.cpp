
#include "Password/PasswordUserWidget.h"

#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ButtonSlot.h"
#include "Components/UniformGridSlot.h"

// ��ʼ�� Widget
void UPasswordUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // �󶨰�ť����¼�
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

// ��������
void UPasswordUserWidget::SetPassword(const FString& NewPassword)
{
    CorrectPassword = NewPassword;
}

// ��֤����
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

// ���ְ�ť����¼�
void UPasswordUserWidget::OnNumberButtonClicked(int32 Number)
{ 
        EnteredPassword.Add(Number);
        UpdatePasswordDisplay();
}

// �����ť����¼�
void UPasswordUserWidget::OnClearButtonClicked()
{
    EnteredPassword.Empty();
    UpdatePasswordDisplay();
}

// ȷ����ť����¼�
void UPasswordUserWidget::OnConfirmButtonClicked()
{
    ValidatePassword();
}

// ����������ʾ
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


