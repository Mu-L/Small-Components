// Fill out your copyright notice in the Description page of Project Settings.


#include "BarCompass/BarCompassWidget.h"
#include "Kismet/GameplayStatics.h"

void UBarCompassWidget::NativeConstruct()
{
    Super::NativeConstruct();
    DM_CompassBar = CompassBar->GetDynamicMaterial();
}

void UBarCompassWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    // 获取玩家控制器
    APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    // 检查玩家控制器是否有效
    if (playerController)
    {
        // 获取玩家的控制旋转并计算Yaw值
        FRotator controlRotation = playerController->GetControlRotation();
        float YawValue = controlRotation.Yaw / 360.0f;

        // 检查动态材质实例是否有效
        if (DM_CompassBar)
        {
            // 设置材质参数值（确保"X(S)"是材质中定义的正确参数名）
            DM_CompassBar->SetScalarParameterValue(TEXT("XY_Float"), YawValue);
        }
    }
}


