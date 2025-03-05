// Fill out your copyright notice in the Description page of Project Settings.


#include "Photo/TakePhotoWidget.h"
#include"Kismet/KismetSystemLibrary.h"
#include"Kismet/GameplayStatics.h"


void UTakePhotoWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (TakePhoto)
    {
        TakePhoto->OnClicked.AddDynamic(this, &UTakePhotoWidget::TakePhotoTest);
    }
}

void UTakePhotoWidget::TakePhotoTest()
{
    // 确保所有控件有效
    if (!ResX || !ResY )
    {
        UE_LOG(LogTemp, Warning, TEXT("One or more widgets are not properly initialized"));
        return;
    }

    // 直接从控件获取输入
    FString ResXString = ResX->GetText().ToString();
    FString ResYString = ResY->GetText().ToString();

  
    // 构建控制台命令
    FString Command = FString::Printf(TEXT("HighResShot %sx%s"), *ResXString, *ResYString);
    UE_LOG(LogTemp, Log, TEXT("%s"), *Command);

    if (APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *Command, OurPlayerController);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No valid player controller found"));
    }
}