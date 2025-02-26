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
    //更新时间与日期
    void UpdateTimeAndDate();

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TimeText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* DateText;

private:
    //时间格式
    FString FormatTime(const FDateTime& Time) const;
    //日期格式
    FString FormatDate(const FDateTime& Time) const;
    FTimerHandle TimerHandle;
};
