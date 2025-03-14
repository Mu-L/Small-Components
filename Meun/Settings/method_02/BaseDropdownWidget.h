#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/ComboBoxString.h"
#include "BaseDropdownWidget.generated.h"

// 将原EWindowMode改为EDisplayWindowMode
UENUM(BlueprintType)
enum class EDisplayWindowMode : uint8
{
    Fullscreen,
    WindowedFullscreen,
    Windowed
};

// 2. 创建基础下拉菜单控件
UCLASS()
class  UBaseDropdownWidget : public UCommonUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UComboBoxString> SettingsComboBox;

    // 初始化枚举类型选项
    template<typename TEnum>
    void InitializeWithEnum(TArray<TEnum> Exclude = {})
    {
        SettingsComboBox->ClearOptions();

        UEnum* EnumPtr = StaticEnum<TEnum>();
        for (int32 i = 0; i < EnumPtr->NumEnums() - 1; ++i)
        {
            if (!Exclude.Contains(static_cast<TEnum>(i)))
            {
                FText DisplayName = EnumPtr->GetDisplayNameTextByIndex(i);
                SettingsComboBox->AddOption(DisplayName.ToString());
            }
        }
    }

    // 初始化自定义选项
    void InitializeWithCustomOptions(const TArray<FString>& Options);

    UFUNCTION(BlueprintCallable)
    FString GetSelectedOption() const;

    UFUNCTION(BlueprintCallable)
    void SetSelectedOption(const FString& Option);
};