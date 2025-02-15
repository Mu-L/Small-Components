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
    // ��ȡ��ҿ�����
    APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    // �����ҿ������Ƿ���Ч
    if (playerController)
    {
        // ��ȡ��ҵĿ�����ת������Yawֵ
        FRotator controlRotation = playerController->GetControlRotation();
        float YawValue = controlRotation.Yaw / 360.0f;

        // ��鶯̬����ʵ���Ƿ���Ч
        if (DM_CompassBar)
        {
            // ���ò��ʲ���ֵ��ȷ��"X(S)"�ǲ����ж������ȷ��������
            DM_CompassBar->SetScalarParameterValue(TEXT("XY_Float"), YawValue);
        }
    }
}


