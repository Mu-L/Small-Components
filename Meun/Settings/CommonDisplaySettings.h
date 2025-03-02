// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include"Components/Button.h"
#include "ViewSettingsStruct.h"
#include "CommonDisplaySettings.generated.h"

// 定义质量等级枚举
enum class EQualityLevel
{
	Low = 0,
	Medium,
	High,
	Epic,
	Cinematic,
	Max // 用于获取等级数量，不是实际等级
};

/**
 * 
 */
UCLASS()
class DEMO_API UCommonDisplaySettings : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	//virtual UWidget* NativeGetDesiredFocusTarget() const override;

	void InitializeComboBox(UComboBoxString* ComboBox, TArray<FString>& Options, FString DefaultOption);
	void SetQualityLevel(UGameUserSettings* UserSettings, FString Option, void (UGameUserSettings::* Setter)(int32), EQualityLevel MinLevel = EQualityLevel::Low, EQualityLevel MaxLevel = EQualityLevel::Cinematic);
	FString GetQualityLevel(int32(UGameUserSettings::* Getter)() const);
	//用户设置
	UPROPERTY()
	TObjectPtr<UGameUserSettings>GameUserSettings;

	//默认设置
	UPROPERTY()
	FViewSettingsStruct DefaultViewSettings;

	//分辨率
	UPROPERTY()
	TArray<FIntPoint>Resolutions;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>ResolutionComboBox;

	//窗口模式
	UPROPERTY()
	TArray<FString>FullscreenMode;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>FullscreenModeComboBox;

	//帧数限制
	UPROPERTY()
	TArray<FString>FrameLimit;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>FrameLimitComboBox;

	//默认开启垂直同步
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCheckBox>VSyncCheckBox;

	//全局光照
	UPROPERTY()
	TArray<FString>GlobalIllumination;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>GlobalIlluminationComboBox;
	 
	//抗锯齿
	UPROPERTY()
	TArray<FString>AntiAliasing;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>AntiAliasingComboBox;

	//反射质量
	UPROPERTY()
	TArray<FString>ReflectionQuality;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>ReflectionQualityComboBox;
	 
	//纹理质量
	UPROPERTY()
	TArray<FString>TextureQuality;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>TextureQualityComboBox;

	//阴影质量
	UPROPERTY()
	TArray<FString>ShadowQuality;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>ShadowQualityComboBox;

	//视觉效果
	UPROPERTY()
	TArray<FString>VisualEffects;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>VisualEffectsComboBox;
	 
	//后期处理
	UPROPERTY()
	TArray<FString>PostProcessing;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>PostProcessingComboBox;

	//植被质量
	UPROPERTY()
	TArray<FString>VegetationQuality;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>VegetationQualityComboBox;

	//着色质量
	UPROPERTY()
	TArray<FString>ColorQuality;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString>ColorQualityComboBox;

	//恢复默认设置按钮
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton>RestoreDefaultsButton;

	//保存设置按钮
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton>SaveButton;

	//更新并应用分辨率
	void InitializeResolutionComboBox();
	UFUNCTION()
	void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//更新并应用窗口模式
	void InitializeFullscreenMode();
	UFUNCTION()
	void OnFullscreenModeSelected(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//更新并应用帧率
	void InitializeFramerate();
	UFUNCTION()
	void OnFramerateChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//更新并应用垂直同步
	void InitializeVSync();
	UFUNCTION()
	void OnVSyncChanged(bool InIsChecked);

	//更新并应用全局光照
	void InitializeGlobalIllumination();
	UFUNCTION()
	void OnGlobalIlluminationChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//更新并应用抗锯齿
	void InitializeAntiAliasing();
	UFUNCTION()
	void OnAntiAliasingChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//更新并应用反射质量
	void InitializeReflectionQuality();
	UFUNCTION()
	void OnReflectionQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//更新并应用纹理质量
	void InitializeTextureQuality();
	UFUNCTION()
	void OnTextureQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//更新并应用阴影质量
	void InitializeShadowQuality();
	UFUNCTION()
	void OnShadowQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//更新并应用视觉效果
	void InitializeVisualEffects();
	UFUNCTION()
	void OnVisualEffectsChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//更新并应用后期处理
	void InitializePostProcessing();
	UFUNCTION()
	void OnPostProcessingChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//更新并应用植被质量
	void InitializeVegetationQuality();
	UFUNCTION()
	void OnVegetationQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	//更新并应用着色质量
	void InitializeColorQuality();
	UFUNCTION()
	void OnColorQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType);

	// 按钮点击事件
	UFUNCTION()
	void OnSaveButtonClicked();

	UFUNCTION()
	void OnRestoreDefaultsButtonClicked();
};
