// Fill out your copyright notice in the Description page of Project Settings.


#include "GetLocalTime/LocalTimeWidget.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Internationalization/Text.h"

void ULocalTimeWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 立即更新一次
    UpdateTimeAndDate();

    //每秒更新一次
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ULocalTimeWidget::UpdateTimeAndDate, 1.0f, true);
}

void ULocalTimeWidget::UpdateTimeAndDate()
{
    const FDateTime CurrentTime = FDateTime::Now();

    if (TimeText)
    {
        TimeText->SetText(FText::FromString(FormatTime(CurrentTime)));
    }

    if (DateText)
    {
        DateText->SetText(FText::FromString(FormatDate(CurrentTime)));
    }
}

FString ULocalTimeWidget::FormatTime(const FDateTime& Time) const
{
    int32 Hour = Time.GetHour();
    FString Period = (Hour < 12) ? TEXT("AM") : TEXT("PM");
    Hour = (Hour % 12) == 0 ? 12 : Hour % 12;

    return FString::Printf(TEXT("%s   %02d:%02d:%02d"),
        *Period,
        Hour,
        Time.GetMinute(),
        Time.GetSecond());
}

FString ULocalTimeWidget::FormatDate(const FDateTime& Time) const
{
    const TCHAR* MonthAbbreviations[] =
    {
        TEXT("JAN."), TEXT("FEB."), TEXT("MAR."),
        TEXT("APR."), TEXT("MAY."), TEXT("JUN."),
        TEXT("JUL."), TEXT("AUG."), TEXT("SEP."),
        TEXT("OCT."), TEXT("NOV."), TEXT("DEC.")
    };

    int32 MonthIndex = Time.GetMonth() - 1;
    MonthIndex = FMath::Clamp(MonthIndex, 0, 11);

    return FString::Printf(TEXT("%s %02d %04d"),
        MonthAbbreviations[MonthIndex],
        Time.GetDay(),
        Time.GetYear());
}