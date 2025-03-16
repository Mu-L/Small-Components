#pragma once

#include "CommonUserWidget.h"
#include "InputSetViewWidget.generated.h"

class UInputSetWidget;
class UInputMappingContext;
class UInputAction;

UCLASS()
class  UInputSetViewWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // ¿∂Õºø…≈‰÷√ Ù–‘
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
    TObjectPtr<UInputMappingContext> DefaultContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
    TObjectPtr<UInputAction> MoveAction;

    // øÿº˛∞Û∂®
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UInputSetWidget> MoveForward;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UInputSetWidget> MoveBackward;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UInputSetWidget> MoveLeft;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UInputSetWidget> MoveRight;




protected:
    UPROPERTY()
    TObjectPtr<UInputMappingContext> SharedContext;

    virtual void NativeConstruct() override;
};