// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "TakePhotoWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API UTakePhotoWidget : public UCommonUserWidget
{
	GENERATED_BODY()

    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr< UEditableTextBox> ResX;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr< UEditableTextBox> ResY;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr< UEditableTextBox> Path;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr< UEditableTextBox> Name;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr <UButton> TakePhoto;

    UFUNCTION(BlueprintCallable)
    void TakePhotoTest();
};
