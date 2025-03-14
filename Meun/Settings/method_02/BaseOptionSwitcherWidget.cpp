#include"BaseOptionSwitcherWidget.h"

void UBaseOptionSwitcherWidget ::NativeConstruct()
{
    Super::NativeConstruct();
    LeftButton->OnClicked.AddDynamic(this, &UBaseOptionSwitcherWidget::OnLeftClicked);
    RightButton->OnClicked.AddDynamic(this, &UBaseOptionSwitcherWidget::OnRightClicked);
    UpdateDisplay();
}

void UBaseOptionSwitcherWidget::OnLeftClicked()
{
    CurrentIndex = (CurrentIndex - 1 + QualityLevels.Num()) % QualityLevels.Num();
    UpdateDisplay();
}

void UBaseOptionSwitcherWidget::OnRightClicked()
{
    CurrentIndex = (CurrentIndex + 1) % QualityLevels.Num();
    UpdateDisplay();
}

void UBaseOptionSwitcherWidget::UpdateDisplay()
{
    if (OptionText && QualityLevels.IsValidIndex(CurrentIndex))
    {
        const FString LevelStr = StaticEnum<EQualityLevel>()->GetDisplayNameTextByValue((int64)QualityLevels[CurrentIndex]).ToString();
        OptionText->SetText(FText::FromString(LevelStr));
    }
}

void UBaseOptionSwitcherWidget::SetCurrentLevel(EQualityLevel NewLevel)
{
    const int32 Index = QualityLevels.IndexOfByKey(NewLevel);
    if (Index != INDEX_NONE)
    {
        CurrentIndex = Index;
        UpdateDisplay();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid quality level: %d"), static_cast<int32>(NewLevel));
    }
}