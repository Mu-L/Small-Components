#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include"Components/Button.h"
#include"Components/TextBlock.h"
#include "BaseOptionSwitcherWidget.generated.h"



UENUM(BlueprintType)
enum class EQualityLevel : uint8
{
    Low,
    Medium,
    High,
    Epic,
    Cinematic
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptionChanged, EQualityLevel, NewLevel);
UCLASS()
class UBaseOptionSwitcherWidget : public UCommonUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UButton> LeftButton;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UButton> RightButton;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> OptionText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EQualityLevel> QualityLevels{ 
        EQualityLevel::Low, 
        EQualityLevel::Medium,
        EQualityLevel::High, 
        EQualityLevel::Epic,
        EQualityLevel::Cinematic };

    int32 CurrentIndex = 0;

    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnLeftClicked();

    UFUNCTION()
    void OnRightClicked();

   
    void UpdateDisplay();

public:
    EQualityLevel GetSelectedLevel() const { return QualityLevels[CurrentIndex]; }
public:
    // 增加初始化验证
    void SetCurrentLevel(EQualityLevel NewLevel);
    
public:
    UPROPERTY(BlueprintAssignable)
    FOnOptionChanged OnOptionChanged;
};