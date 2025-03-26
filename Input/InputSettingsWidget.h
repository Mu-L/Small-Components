#pragma once

#include "CommonUserWidget.h"
#include "Components/ScrollBox.h"
#include "KeySettingsWidget.h" // ��Ӱ���
#include "InputSettingsWidget.generated.h"

UCLASS()
class UInputSettingsWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox;

	UPROPERTY(BlueprintReadWrite)
	class UEnhancedInputUserSettings* InputUserSettings;
	UPROPERTY(BlueprintReadWrite)
	FPlayerKeyMapping CurrentKeyMap;
protected:
	virtual void NativeConstruct() override;
	
};