#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "BaseOptionSwitcherWidget.h"
#include "BaseDropdownWidget.h"
#include "VideoSettingsWidget.generated.h"


UCLASS()
class UVideoSettingsWidget : public UCommonUserWidget
{
    GENERATED_BODY()
protected:
    virtual void NativeConstruct() override;
public:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UBaseOptionSwitcherWidget> QuickSetQualitySwitcher;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UBaseOptionSwitcherWidget> ShadowQualitySwitcher;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UBaseOptionSwitcherWidget> VisualEffectsSwitcher;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UBaseOptionSwitcherWidget> PostProcessingSwitcher;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UBaseOptionSwitcherWidget> FoliageQualitySwitcher;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UBaseOptionSwitcherWidget> ShadingQualitySwitcher;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UBaseOptionSwitcherWidget> GlobalIlluminationQualitySwitcher;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UBaseOptionSwitcherWidget> AntiAliasingQualitySwitcher;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UBaseOptionSwitcherWidget> ReflectionQualitySwitcher;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UBaseOptionSwitcherWidget> TextureQualitySwitcher;

    // 新增下拉菜单组件
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UBaseDropdownWidget> WindowModeDropdown;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UBaseDropdownWidget> ResolutionDropdown;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UBaseDropdownWidget> FrameRateDropdown;

    UFUNCTION(BlueprintCallable)
    void SaveSettings();

    UFUNCTION(BlueprintCallable)
    void RestoreDefaults();

    UFUNCTION(BlueprintCallable)
    void OnQuickSetChanged(EQualityLevel NewLevel);

private:
    // 分辨率缓存
    TArray<FIntPoint> AvailableResolutions;
    TArray<FString> CachedResolutionStrings;
};