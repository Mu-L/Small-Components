#include"SelectionBase.h"
#include"CommonTextBlock.h"
#include"Logging/StructuredLog.h"

USelectionBase::USelectionBase()
{
	//���õ�ǰѡ��Ϊ0
	CurrentSelection = 0;

	//�������Ծ۽�
	SetIsFocusable(true);

	//���ö���ɼ���Ϊ�ɼ�
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
	//���� Options ���飬�Ƴ�����ѡ�
	Options.Reset();
}

void USelectionBase::AddOption(const FSelectionOption& InOption)
{
	//�� Options ���������һ���µ�ѡ�
	Options.Add(InOption);
	//���� UpdateCurrentSelection ���������µ�ǰѡ��
	UpdateCurrentSelection();
}

void USelectionBase::SetCurrentSelection(int InIndex)
{
	check(InIndex >= 0 && InIndex <Options.Num());
	//���õ�ǰѡ��ָ����������
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

	//����ѭ��ѡ�񣨵�ѡ�񳬳���Χʱ�ص���һ�ˣ���
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
	//��鵱ǰѡ���Ƿ���Ч��
	if (CurrentSelection < 0 || CurrentSelection >= Options.Num())
	{
		// Handle the error, e.g., log an error message and return
		//UE_LOG(LogTemp, Error, TEXT("UpdateCurrentSelection Widget: Invalid array index: %d"), CurrentSelection);
		return;
	}
	//������ʾ�ı�ǩΪ��ǰѡ����ı�ǩ
	Label->SetText(Options[CurrentSelection].Label);
}
