#pragma once

#include "CoreMinimal.h"
#include"UObject/Object.h"
#include"CommonButtonBase.h"
#include"SelectionOption.h"

#include "SelectionBase.generated.h"

// ����һ��ί�����ͣ���ί�н���һ����������
DECLARE_DELEGATE_OneParam(FOnSelectionChange, int);

class UCommonTextBlock;
     
UCLASS()
class MYPROJECT_API USelectionBase :public UCommonUserWidget
{
	GENERATED_BODY()
public:
	USelectionBase();
	virtual void NativeConstruct()override;

	void Clear();
	void AddOption(const FSelectionOption& InOption);
	void SetCurrentSelection(int InIndex);


	UFUNCTION(BlueprintCallable)
	void SelectionPrevious();

	UFUNCTION(BlueprintCallable)
	void SelectNext();

	
	FOnSelectionChange OnSelectionChange;
protected:
	UFUNCTION()
	UWidget* OnNavigation(EUINavigation InNavigation);

	void UpdateCurrentSelection();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<FSelectionOption>Options;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UCommonTextBlock>Label;

	int CurrentSelection;
};