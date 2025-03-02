#pragma once
#include "CoreMinimal.h"
#include"CommonActivatableWidget.h"
#include"CommonButtonBase.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include"SelectionBase.h"
#include"SettingWidget2.generated.h"



UCLASS()
class MYPROJECT_API USettingWidget2 :public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

protected:
	void InitializeResolutionComboBox();
	void InitializeVSync();
	void InitializeFramerate();

	UFUNCTION()
	void OnResolutionChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType);

	UFUNCTION()
	void OnVSyncChanged(bool InIsChecked);

	UPROPERTY()
	TObjectPtr<UGameUserSettings>GameUserSettings;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UComboBoxString>ResolutionComboBox;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UCheckBox>VSyncCheckBox;

	//帧数设置
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USelectionBase>FramerateSelection;

	//着色质量
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USelectionBase>ShadingQualitySelection;

	//全局照明
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USelectionBase>GlobalIlluminationQualitySelection;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USelectionBase>PostProcessQualitySelection;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USelectionBase>VisualEffectQualitySelection;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USelectionBase>ShadowQualitySelection;

	UPROPERTY()
	TArray<FIntPoint>Resolutions;
};