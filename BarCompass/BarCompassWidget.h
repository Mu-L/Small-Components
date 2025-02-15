// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include"Components/Image.h"
#include "BarCompassWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API UBarCompassWidget : public UCommonUserWidget
{
	GENERATED_BODY()
protected:
    // 初始化 Widget
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
    // 绑定组件
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage>CompassBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage>Marker;

    class UMaterialInstanceDynamic* DM_CompassBar;
};
