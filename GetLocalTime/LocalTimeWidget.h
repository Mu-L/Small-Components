// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "LocalTimeWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API ULocalTimeWidget : public UCommonUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

protected:
    //����ʱ��������
    void UpdateTimeAndDate();

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TimeText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* DateText;

private:
    //ʱ���ʽ
    FString FormatTime(const FDateTime& Time) const;
    //���ڸ�ʽ
    FString FormatDate(const FDateTime& Time) const;
    FTimerHandle TimerHandle;
};
