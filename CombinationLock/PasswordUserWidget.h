// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include"Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "PasswordUserWidget.generated.h"


/**
 * 
 */
UCLASS()
class DEMO_API UPasswordUserWidget : public UCommonUserWidget
{
	GENERATED_BODY()
public:
    // ��������
    UFUNCTION(BlueprintCallable, Category = "PasswordLock")
    void SetPassword(const FString& NewPassword);

    // ��֤����
    UFUNCTION(BlueprintCallable, Category = "PasswordLock")
    bool ValidatePassword();

    //����������ʾ
    void UpdatePasswordDisplay();

protected:
    // ��ʼ�� Widget
    virtual void NativeConstruct() override;

private:
    // �����
    UPROPERTY(meta = (BindWidget))
    class UUniformGridPanel* NumberGridPanel;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PasswordDisplayText;

    UPROPERTY(meta = (BindWidget))
    class UButton* ConfirmButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ClearButton;

    // ���ְ�ť����¼�
    UFUNCTION(BlueprintCallable)
    void OnNumberButtonClicked(int32 Number);

    // �����ť����¼�
    UFUNCTION()
    void OnClearButtonClicked();

    // ȷ����ť����¼�
    UFUNCTION()
    void OnConfirmButtonClicked();

    // �洢�û����������
    UPROPERTY()
    TArray<int32> EnteredPassword;

    // �洢��ȷ����
    UPROPERTY()
    FString CorrectPassword;

};

