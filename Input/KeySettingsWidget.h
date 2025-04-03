#pragma once

#include "CommonUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "KeySettingsWidget.generated.h"

class UInputKeySelector;
class EnhancedInputUserSettings;

UCLASS()
class UKeySettingsWidget : public UCommonUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> KeyName;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UInputKeySelector> InputKeySelector;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* Button_Reset;

    UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
    FPlayerKeyMapping KeyMapping;

    UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
    UEnhancedInputUserSettings* InputUserSettings;
protected:
    void UpdateInputKeySeletorbyKeyMapping(FKey Key);
    UFUNCTION()
    void OnKeySelected(FInputChord SelectedKey);
    UFUNCTION()
    void OnIsSelectingKeyChanged();
    UFUNCTION()
    void OnResetClicked();
private:
    bool bIsKeyChanged;
};