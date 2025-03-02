#include"SelectionBase.h"
#include"CommonTextBlock.h"
#include"Logging/StructuredLog.h"

USelectionBase::USelectionBase()
{
	//设置当前选择为0
	CurrentSelection = 0;

	//开启可以聚焦
	SetIsFocusable(true);

	//设置对象可见性为可见
	SetVisibilityInternal(ESlateVisibility::Visible);
}

void USelectionBase::NativeConstruct()
{
	Super::NativeConstruct();
	if (Options.Num() == 0)
	{
		UE_LOGFMT(LogTemp, Log, "USelectionBase:No options where provied.");
		return;
	}
	UpdateCurrentSelection();

	FCustomWidgetNavigationDelegate NavigationDelegate;
	NavigationDelegate.BindDynamic(this, &USelectionBase::OnNavigation);
	SetNavigationRuleCustom(EUINavigation::Left, NavigationDelegate);
	SetNavigationRuleCustom(EUINavigation::Right, NavigationDelegate);
}

void USelectionBase::Clear()
{
	//重置 Options 数组，移除所有选项。
	Options.Reset();
}

void USelectionBase::AddOption(const FSelectionOption& InOption)
{
	//向 Options 数组中添加一个新的选项。
	Options.Add(InOption);
	//调用 UpdateCurrentSelection 方法来更新当前选择
	UpdateCurrentSelection();
}

void USelectionBase::SetCurrentSelection(int InIndex)
{
	check(InIndex >= 0 && InIndex <Options.Num());
	//设置当前选择到指定的索引。
	CurrentSelection = InIndex;
	UpdateCurrentSelection();
}

void USelectionBase::SelectionPrevious()
{
	OnNavigation(EUINavigation::Left);
}

void USelectionBase::SelectNext()
{
	OnNavigation(EUINavigation::Right);
}


UWidget* USelectionBase::OnNavigation(EUINavigation InNavigation)
{
	check(InNavigation == EUINavigation::Left || InNavigation == EUINavigation::Right);
	const auto Direction = InNavigation == EUINavigation::Left ? -1 : 1;

	CurrentSelection += Direction;

	//处理循环选择（当选择超出范围时回到另一端）。
	if (CurrentSelection < 0)
	{
		CurrentSelection = Options.Num() - 1;
	}
	else if (CurrentSelection >= Options.Num())
	{
		CurrentSelection = 0;
	}

	UpdateCurrentSelection();

	OnSelectionChange.ExecuteIfBound(CurrentSelection);
	return this;
}

void USelectionBase::UpdateCurrentSelection()
{
	//检查当前选择是否有效。
	if (CurrentSelection < 0 || CurrentSelection >= Options.Num())
	{
		// Handle the error, e.g., log an error message and return
		//UE_LOG(LogTemp, Error, TEXT("UpdateCurrentSelection Widget: Invalid array index: %d"), CurrentSelection);
		return;
	}
	//更新显示的标签为当前选择项的标签
	Label->SetText(Options[CurrentSelection].Label);
}
