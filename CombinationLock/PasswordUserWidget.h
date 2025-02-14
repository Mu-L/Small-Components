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
    // 设置密码
    UFUNCTION(BlueprintCallable, Category = "PasswordLock")
    void SetPassword(const FString& NewPassword);

    // 验证密码
    UFUNCTION(BlueprintCallable, Category = "PasswordLock")
    bool ValidatePassword();

    //更新密码显示
    void UpdatePasswordDisplay();

protected:
    // 初始化 Widget
    virtual void NativeConstruct() override;

private:
    // 绑定组件
    UPROPERTY(meta = (BindWidget))
    class UUniformGridPanel* NumberGridPanel;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PasswordDisplayText;

    UPROPERTY(meta = (BindWidget))
    class UButton* ConfirmButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ClearButton;

    // 数字按钮点击事件
    UFUNCTION(BlueprintCallable)
    void OnNumberButtonClicked(int32 Number);

    // 清除按钮点击事件
    UFUNCTION()
    void OnClearButtonClicked();

    // 确定按钮点击事件
    UFUNCTION()
    void OnConfirmButtonClicked();

    // 存储用户输入的密码
    UPROPERTY()
    TArray<int32> EnteredPassword;

    // 存储正确密码
    UPROPERTY()
    FString CorrectPassword;

};

